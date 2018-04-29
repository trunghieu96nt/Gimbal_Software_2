#ifndef PID_H
#define PID_H

#include <QtMath>

class PID
{
public:
    explicit PID();
    void PIDSet(float setpoint, bool use_ramp);
    float PIDCalc(float feedback, float max_response,float dT);
    void PIDReset();
    void PIDSetParams(float Kp, float Ki, float Kd, float Kff1, float Kff2);
    void PIDSetFeedforward(float Kff1, float Kff2);
    //PID_set_filter();
    void PIDEnable();
    void PIDDisable();
    void PIDSetAlphaDerivative(float alpha);
    void PIDSetAlphaFeedpart2(float alpha);
    void PIDSetKp(float Kp);
    void PIDSetKi(float Ki);
    void PIDSetKd(float Kd);
    void PIDSetKff1(float Kff1);
    void PIDSetKff2(float Kff2);
    void PIDSetMaxSetpointInc(float maxSetPointInc);
    void PIDSetMaxSetpoint(float maxSetpoint);
private:
    float preSetpoint2;
    float preSetpoint;
    float setpoint;
    float diffSetpoint;
    float deadband;
    float maxSetpoint;
    float maxSetpointInc;

    float PIDError;
    float PIDErrorTemp1;
    float PIDErrorTemp2;

    float Kp;
    float Ki;
    float Kd;
    float Kff1;/* feedforward */
    float Kff2;/* feedforward diff */
    float feedPart1;
    float feedPart2;
    float alphaFeedPart2;
    float pPart;
    float iPart;
    float dPart;
    float derivative;
    float alphaDerivative;

    float PIDResult;
    float PIDResultTemp;

    bool enable;

    float saturate(float input, float max_value, float min_value);
};

#endif // PID_H
