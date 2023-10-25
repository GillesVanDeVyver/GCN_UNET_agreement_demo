#pragma once

#include "FAST/Visualization/Window.hpp"
#include <QLabel>
#include <FAST/Visualization/ImageRenderer/ImageRenderer.hpp>
#include <FAST/Visualization/SegmentationRenderer/SegmentationRenderer.hpp>
#include "AgreementCalculator.hpp"
#include <FAST/Visualization/TextRenderer/TextRenderer.hpp>

namespace fast {

    class CustomGUI : public Window {
    FAST_OBJECT_V4(CustomGUI)


    public:
        FAST_CONSTRUCTOR(CustomGUI,
                         std::shared_ptr<ImageRenderer>, imageRenderer_left,,
                         std::shared_ptr<SegmentationRenderer>, segmentationRenderer_left,,
                         std::shared_ptr<ImageRenderer>, imageRenderer_right,,
                         std::shared_ptr<SegmentationRenderer>, segmentationRenderer_right,,
                         std::shared_ptr<AgreementCalculator>, agreementCalculator,,
                         std::shared_ptr<TextRenderer>, agreementRenderer,,
                         int, frameRate, =5

        )
    private:
    };

} // end namespace fast