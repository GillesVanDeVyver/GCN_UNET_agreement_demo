#pragma once

#include <FAST/ProcessObject.hpp>



namespace fast {
    class DistanceNetProcessor : public ProcessObject {
    FAST_PROCESS_OBJECT(DistanceNetProcessor);
    public:
        FAST_CONSTRUCTOR(DistanceNetProcessor,
                         bool, convert_to_mesh, = false

        );
    private:
        void init();
        void execute() override;
        bool convert_to_mesh = false;
        float scale_factor_mesh = 150.f;
    };
}