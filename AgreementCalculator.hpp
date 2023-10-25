#pragma once
#include <QProgressBar>
#include <FAST/ProcessObject.hpp>
#include <QLabel>
#include "CustomBar.hpp"


namespace fast {
    class AgreementCalculator : public ProcessObject {
    FAST_PROCESS_OBJECT(AgreementCalculator);
    public:
        FAST_CONSTRUCTOR(AgreementCalculator,
                         CustomBar*, statusBarIn,=nullptr);
        CustomBar *agreementBar;

    private:
        void init();
        void execute() override;
    };
}