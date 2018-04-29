#include "pid.h"

PID::PID()
{
    this->preSetpoint2=0.0f;
    this->preSetpoint=0.0f;
    this->setpoint=0.0f;
    this->diffSetpoint=0.0f;
    this->deadband=0.0f;

    this->PIDError=0.0f;
    this->PIDErrorTemp1=0.0f;
    this->PIDErrorTemp2=0.0f;

    this->Kp=0.0f;
    this->Ki=0.0f;
    this->Kd=0.0f;
    this->Kff1=0.0f;//feedforward
    this->Kff2=0.0f;
    this->pPart=0.0f;
    this->iPart=0.0f;
    this->dPart=0.0f;
    this->derivative=0.0f;
    this->alphaDerivative=1.0f;
    this->feedPart1=0.0f;
    this->feedPart2=0.0f;
    this->alphaFeedPart2=0.0f;

    this->PIDResult=0.0f;
    this->PIDResultTemp=0.0f;

    this->enable=0;
}

float PID::saturate(float input, float max_value, float min_value)
{
    if (input > max_value)
    {
        input = max_value;
    }
    else if (input < min_value)
    {
        input = min_value;
    }
    return input;
}

void PID::PIDSet(float setpoint, bool useRamp)
{
    this->preSetpoint2 = 	this->preSetpoint;
    this->preSetpoint = 	this->setpoint;
    setpoint = saturate(setpoint, this->maxSetpoint,-this->maxSetpoint);
    if (useRamp)
    {
        if (setpoint - this->preSetpoint > this->maxSetpointInc)
            this->setpoint += this->maxSetpointInc;
        else if (setpoint - this->preSetpoint < -1*this->maxSetpointInc)
            this->setpoint -= this->maxSetpointInc;
        else
            this->setpoint = setpoint;
    }
    else
    {
        this->setpoint = setpoint;
    }
}

float PID::PIDCalc(float feedback, float max_response, float dT)
{
    float derivative;
    this->PIDError = this->setpoint - feedback;
    if (qAbs(this->PIDError) < this->deadband)
    {
        this->PIDError = 0;
    }

    this->pPart = this->Kp * this->PIDError;
    this->iPart += this->Ki * dT * (this->PIDError + this->PIDErrorTemp1)/2;

    if (this->iPart > max_response)
        this->iPart = max_response;
    else if (this->iPart < -max_response)
        this->iPart = -max_response;

    derivative = (this->PIDError - this->PIDErrorTemp1)/dT;
    this->derivative = this->derivative + this->alphaDerivative * (derivative - this->derivative);
    this->dPart = this->Kd * (this->derivative);

    if (this->dPart > max_response/2)
        this->dPart = max_response/2;
    else if (this->dPart < -max_response/2)
        this->dPart = -max_response/2;

    this->PIDErrorTemp1=this->PIDError;
    this->PIDResult = this->pPart + this->iPart + this->dPart;

    this->feedPart1 = this->Kff1*this->setpoint;

    diffSetpoint = this->setpoint - this->preSetpoint;

    this->feedPart2 = this->feedPart2 +	this->alphaFeedPart2*(this->Kff2*diffSetpoint-this->feedPart2);

    if (this->feedPart2 > max_response)
        this->feedPart2 = max_response;
    else if (this->feedPart2 < -max_response)
        this->feedPart2 = -max_response;

    this->PIDResult += this->feedPart1 + this->feedPart2;
    this->preSetpoint = this->setpoint;

    if (this->PIDResult > max_response)
        this->PIDResult = max_response;
    else if (this->PIDResult < -max_response)
        this->PIDResult = -max_response;

    return this->PIDResult;
}

void PID::PIDReset()
{
    this->PIDErrorTemp1=0.0f;
    this->PIDErrorTemp2=0.0f;
    this->PIDResultTemp=0.0f;
    this->iPart = 0.0f;
    this->setpoint = 0.0f;
    this->preSetpoint = 0.0f;
    this->preSetpoint2 = 0.0f;
    this->derivative = 0.0f;
    this->diffSetpoint = 0.0f;
    this->feedPart2 = 0.0f;
}

void PID::PIDSetParams(float Kp, float Ki, float Kd, float Kff1, float Kff2)
{
    this->Kp=Kp;
    this->Ki=Ki;
    this->Kd=Kd;
    this->Kff1=Kff1;
    this->Kff2=Kff2;
}

void PID::PIDSetFeedforward(float Kff1, float Kff2)
{
    this->Kff1 = Kff1;
    this->Kff2 = Kff2;
}

void PID::PIDEnable()
{
    this->enable = 1;
    PIDReset();
}

void PID::PIDDisable()
{
    this->enable = 0;
}

void PID::PIDSetAlphaDerivative(float alpha)
{
    this->alphaDerivative = alpha;
}

void PID::PIDSetAlphaFeedpart2(float alpha)
{
    this->alphaFeedPart2 = alpha;
}

void PID::PIDSetKp(float Kp)
{
    this->Kp = Kp;
}

void PID::PIDSetKi(float Ki)
{
    this->Ki = Ki;
}

void PID::PIDSetKd(float Kd)
{
    this->Kd = Kd;
}

void PID::PIDSetKff1(float Kff1)
{
    this->Kff1 = Kff1;
}

void PID::PIDSetKff2(float Kff2)
{
    this->Kff1 = Kff2;
}

void PID::PIDSetMaxSetpointInc(float maxSetpointInc)
{
    this->maxSetpointInc = maxSetpointInc;
}

void PID::PIDSetMaxSetpoint(float maxSetpoint)
{
    this->maxSetpoint = maxSetpoint;
}
