#include "AgreementCalculator.hpp"
#include <FAST/Data/Image.hpp>
#include "FAST/Data/Tensor.hpp"
#include "FAST/Data/Mesh.hpp"
#include <vector>
#include "FAST/Data/Text.hpp"
#include <chrono>
#include <iostream>


namespace fast {

    void AgreementCalculator::init() {
        createInputPort<Image>(0);
        createInputPort<Image>(1);
        createOutputPort<std::vector<float>>(0);
    }

    AgreementCalculator::AgreementCalculator(CustomBar* agreementBarIn) {
        init();
        agreementBar=agreementBarIn;
    }

    // Calculate the Dice coefficient between two segmentation maps
    double diceCoefficient(std::vector<uchar> segMap1, std::vector<uchar> segMap2) {
        if (segMap1.size() != segMap2.size()) {
            std::cerr << "Error: Segmentation maps should have the same size." << std::endl;
            return -1.0;
        }

        int intersectionCount = 0;

        int nonZeroCount1=0;
        int nonZeroCount2=0;
        for (size_t i = 0; i < segMap1.size(); ++i) {

            int label1 = segMap1[i];
            int label2 = segMap2[i];

            if (label1!='\000') {
                ++nonZeroCount1;
            }
            if (label2!='\000') {
                ++nonZeroCount2;
            }

            // label 1 and 2 are flipped becuase of the conversion from keypoints to segmentation map for the gcn
            if (label1!='\000'){
                if ((label1 == '\001' && label2 == '\002')
                || (label1 == '\002' && label2 == '\001')
                || (label1 == '\003' && label2 == '\003')){
                    ++intersectionCount;
                }
            }
        }
        // Calculate the Dice coefficient
        double diceCoeff = 2.0 * intersectionCount / (nonZeroCount1 + nonZeroCount2);
        return diceCoeff;
    }


    void AgreementCalculator::execute() {

        auto seg1 = getInputData<Image>(0);
        auto seg2 = getInputData<Image>(1);

        auto seg1_acces = seg1->getImageAccess(ACCESS_READ);
        auto seg2_acces = seg2->getImageAccess(ACCESS_READ);

        auto seg1_data=seg1_acces->get();
        auto seg2_data=seg2_acces->get();

        auto* segMap1 = (uchar*)seg1_data;
        auto* segMap2 = (uchar*)seg2_data;

        auto nb_voxels= seg1->getNrOfVoxels();

        auto dice = diceCoefficient(std::vector<uchar>(segMap1, segMap1 + nb_voxels), std::vector<uchar>(segMap2, segMap2 + nb_voxels));

        agreementBar->setValue(dice);

        //std::string dice_as_str = std::to_string(dice);
        //auto text = Text::create(dice_as_str);
        std::shared_ptr<Text> dummyText = Text::create("");
        addOutputData(0, dummyText);


    }


}