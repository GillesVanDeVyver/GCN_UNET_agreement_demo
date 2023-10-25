#include "CustomGUI.hpp"
#include <FAST/Algorithms/NeuralNetwork/SegmentationNetwork.hpp>
#include <vector>
#include "AgreementCalculator.hpp"
#include "CustomBar.hpp"

namespace fast {

    CustomGUI::CustomGUI(std::shared_ptr<ImageRenderer> imageRenderer_left,
                         std::shared_ptr<SegmentationRenderer> segmentationRenderer_left,
                         std::shared_ptr<ImageRenderer> imageRenderer_right,
                         std::shared_ptr<SegmentationRenderer> segmentationRenderer_right,
                         std::shared_ptr<AgreementCalculator> agreementCalculator,
                         std::shared_ptr<TextRenderer> agreementRenderer,
                         int framerate) {

        //enableFullscreen();

        auto viewSize=900;
        View* view_left = createView();
        view_left->set2DMode();
        view_left->setFixedHeight(viewSize);
        view_left->setFixedWidth(viewSize);
        view_left->addRenderer(imageRenderer_left);
        view_left->addRenderer(segmentationRenderer_left);


        View* view_right= createView();
        view_right->set2DMode();
        view_right->setFixedHeight(viewSize);
        view_right->setFixedWidth(viewSize);
        view_right->addRenderer(imageRenderer_right);
        view_right->addRenderer(segmentationRenderer_right);
        view_right->addRenderer(agreementRenderer);


        auto customBarLayout = new QHBoxLayout;
        customBarLayout->setAlignment(Qt::AlignHCenter);
        auto customBar = agreementCalculator->agreementBar;
        customBar->setFixedHeight(50);
        customBarLayout->addWidget(customBar);

        auto dualViewLayout = new QHBoxLayout;
        dualViewLayout->addWidget(view_left);
        dualViewLayout->addWidget(view_right);

        auto mainLayout = new QHBoxLayout;
        mainLayout->setDirection(QHBoxLayout::TopToBottom);
        mainLayout->addLayout(customBarLayout);
        mainLayout->addLayout(dualViewLayout);
        setCenterLayout(mainLayout);


        view_left->setBackgroundColor(Color::Black());
        view_right->setBackgroundColor(Color::Black());
        // Set up a black background for the entire application
        QPalette palette;
        palette.setColor(QPalette::Background, Qt::black);

        // Apply the palette to the application
        qApp->setPalette(palette);
    }


}