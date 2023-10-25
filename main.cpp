#include <FAST/Importers/ImageFileImporter.hpp>
#include <FAST/Visualization/ImageRenderer/ImageRenderer.hpp>
#include <FAST/Visualization/SimpleWindow.hpp>
#include <FAST/Data/Tensor.hpp>
#include <FAST/Algorithms/NeuralNetwork/SegmentationNetwork.hpp>
#include <FAST/Tools/CommandLineParser.hpp>
#include <FAST/Streamers/ImageFileStreamer.hpp>
#include <FAST/Visualization/SegmentationRenderer/SegmentationRenderer.hpp>
#include <FAST/Algorithms/NeuralNetwork/InferenceEngineManager.hpp>
#include <FAST/Algorithms/Color/GrayscaleToColor.hpp>
#include "DistanceNetProcessor.hpp"
#include <vector>
#include <FAST/Algorithms/NeuralNetwork/VertexTensorToSegmentation.hpp>
#include "AgreementCalculator.hpp"
#include <FAST/Visualization/TextRenderer/TextRenderer.hpp>
#include "CustomGUI.hpp"
#include "CustomBar.hpp"
#include <iostream>
#include <filesystem>
#include <FAST/Algorithms/Lambda/RunLambda.hpp>

using namespace fast;

int main(int argc, char** argv) {

    // Reducing the framerate might be necessary on low end GPUs
    int framerate=60;

    // assuming working directory is build
    std::cout << "Current working directory: " << std::filesystem::current_path() << '\n';


    // Using examples file from the FAST test data as default
    std::string default_mhd_file_location=Config::getTestDataPath() + "/US/Heart/ApicalTwoChamber/US-2D_#.mhd";
    //std::string default_mhd_file_location=Config::getTestDataPath() + "/US/Heart/ApicalLongAxis/US-2D_#.mhd"; //out of distribution

    // Using models trained on first cross validation split of CAMUS as default
    auto default_gcn_model_loc=
            "../models/GCN_camus_cv1.onnx";
    auto default_unet_model_loc =
            "../models/nnunet_camus_cv1.onnx";

    std::string mhd_file_location;
    std::string gcn_model_loc;
    std::string unet_model_loc;
    CommandLineParser parser("Real-time GCN-UNet agreement");
    parser.addVariable("mhd_file_location",default_mhd_file_location);
    parser.addVariable("gcn_model_loc",default_gcn_model_loc);
    parser.addVariable("unet_model_loc",default_unet_model_loc);
    parser.parse(argc, argv);
    mhd_file_location=parser.get("mhd_file_location");
    gcn_model_loc=parser.get("gcn_model_loc");
    unet_model_loc=parser.get("unet_model_loc");

    InferenceEngine::pointer engine;
    engine = InferenceEngineManager::loadEngine("ONNXRuntime");

    // stream form disk
    auto streamer = ImageFileStreamer::create(mhd_file_location,true,true, framerate);

    // stream from scanner: this requires a an active OpenIGTLink connection with the scanner
    //auto streamer = OpenIGTLinkStreamer::create();

    // fix for bug where streamer is used by 2 pipelines
    // if this gives problems, contact Erik Smistad (https://www.eriksmistad.no/)
    auto copy = RunLambda::create([](DataObject::pointer d) {
        auto image = std::dynamic_pointer_cast<Image>(d);
        auto image2 = image->copy(Host::getInstance());
        return DataList(image2);
    })->connect(streamer);

    auto imageRenderer_right = ImageRenderer::create()
            ->connect(copy);
    auto imageRenderer_left = ImageRenderer::create()
            ->connect(streamer);
    // U-Net
    auto segmentation_unet = SegmentationNetwork::create(unet_model_loc,{}, {},
                                                    engine->getName())
            ->connect(copy);
    // normalize to [0,1]
    //segmentation_unet->setScaleFactor(1.0f/255.0f);

    // GCN
    auto preprocessor = GrayscaleToColor::create(false)->connect(streamer);
    auto gcn = NeuralNetwork::create(gcn_model_loc,
                                     {}, {}, engine->getName())
            ->connect(preprocessor);
    // normalize to [0,1]
    gcn->setScaleFactor(1.0f/255.0f);
    gcn->setMeanAndStandardDeviation(0.5f, 0.5f);
    auto gcn_pp = DistanceNetProcessor::create()->
            connect(0, gcn, 0)->connect(1, gcn, 1);

    // define graph connections
    auto nb_lv_points = 43;
    auto nb_la_points = 21;
    auto nb_ep_points = 43;
    auto total_inner = nb_lv_points+nb_la_points;
    std::vector<MeshLine> connections0;
    std::vector<MeshLine> connections1;
    std::vector<MeshLine> connections2;
    for (int i = 0; i < nb_lv_points-1; ++i) {
        connections0.emplace_back(i, i+1);
    }
    connections0.emplace_back(nb_lv_points-1,0 );

    for (int i = nb_lv_points-1; i < total_inner-1; ++i) {
        connections1.emplace_back(i, i+1);
    }
    connections1.emplace_back(total_inner-1,0 );
    connections1.emplace_back(0, nb_lv_points-1);

    for (int i = total_inner; i < total_inner+nb_ep_points-2; ++i) {
        connections2.emplace_back(i, i+1);
    }
    connections2.emplace_back(total_inner+nb_ep_points-2,total_inner);
    auto connections=std::vector<std::vector<MeshLine>>();
    connections.emplace_back(connections2);
    connections.emplace_back(connections0);
    connections.emplace_back(connections1);

    // convert vertices in graph to segmentation mask
    auto tensor2seg = VertexTensorToSegmentation::create(connections)->connect(gcn_pp);

    // Render segmentation masks
    auto segmentationRenderer_left = SegmentationRenderer::create({{1, Color::Blue()},
                                                                    {2, Color::Red()}
                                                                           , {3, Color::Green()}}, 0.25)
            ->connect(tensor2seg);
    auto segmentationRenderer_right = SegmentationRenderer::create({{1, Color::Red()},
                                                                    {2, Color::Blue()}
                                                                           , {3, Color::Green()}}, 0.25)
            ->connect(segmentation_unet);

    // Calculate agreement
    auto agreementBar = new CustomBar(nullptr,0.5);
    auto agreement_calculator = AgreementCalculator::create(agreementBar)->connect(0, segmentation_unet, 0)
            ->connect(1, tensor2seg, 0);

    auto agreementRenderer = TextRenderer::create()->connect(agreement_calculator);


    // GUI
    auto customGUI = CustomGUI::create(imageRenderer_left,segmentationRenderer_left,
                                       imageRenderer_right,segmentationRenderer_right,
                                       agreement_calculator,
                                       agreementRenderer,framerate);
    customGUI->enableFullscreen();
    customGUI->run();

}
