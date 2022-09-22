"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.RobotType = exports.EAIType = exports.EEventNotifyType = exports.EJoinUpdateType = exports.ERobotState = exports.ESkillType = void 0;
/**机器人技能类型 */
var ESkillType;
(function (ESkillType) {
    ESkillType[ESkillType["Skill_None"] = 0] = "Skill_None";
    ESkillType[ESkillType["Skill_Move"] = 1] = "Skill_Move";
    ESkillType[ESkillType["Skill_Action"] = 2] = "Skill_Action";
    ESkillType[ESkillType["Skill_TTS"] = 3] = "Skill_TTS";
    ESkillType[ESkillType["Skill_TakeMeTo"] = 4] = "Skill_TakeMeTo";
    ESkillType[ESkillType["Skill_DanceAndSing"] = 5] = "Skill_DanceAndSing";
    ESkillType[ESkillType["Skill_CheckEnvironment"] = 6] = "Skill_CheckEnvironment";
    ESkillType[ESkillType["Skill_AnomalyHandle"] = 7] = "Skill_AnomalyHandle";
    ESkillType[ESkillType["Skill_FaceDetect"] = 8] = "Skill_FaceDetect";
    ESkillType[ESkillType["Skill_QA"] = 9] = "Skill_QA";
    ESkillType[ESkillType["Skill_NaviTo"] = 10] = "Skill_NaviTo";
})(ESkillType = exports.ESkillType || (exports.ESkillType = {}));
/**
 * 机器人角色基类
 */
var ERobotState;
(function (ERobotState) {
    /**待机状态 */
    ERobotState[ERobotState["Idle"] = 0] = "Idle";
    /**播放TTS语音状态 */
    ERobotState[ERobotState["TTSing"] = 1] = "TTSing";
    /**行走状态 */
    ERobotState[ERobotState["Moving"] = 3] = "Moving";
    /**播放动作 舞蹈等*/
    ERobotState[ERobotState["Acting"] = 4] = "Acting";
    /**充电状态 */
    ERobotState[ERobotState["Charging"] = 5] = "Charging";
    /**遥控状态 */
    ERobotState[ERobotState["ControlStatus"] = 6] = "ControlStatus";
})(ERobotState = exports.ERobotState || (exports.ERobotState = {}));
/**机器人的灵动模式更新类型**/
var EJoinUpdateType;
(function (EJoinUpdateType) {
    /**灵动模式启动事件*/
    EJoinUpdateType[EJoinUpdateType["StartReady"] = 0] = "StartReady";
    /**灵动待机*/
    EJoinUpdateType[EJoinUpdateType["Join_Idle"] = 1] = "Join_Idle";
    /**灵动唱歌*/
    EJoinUpdateType[EJoinUpdateType["Join_Sing"] = 2] = "Join_Sing";
})(EJoinUpdateType = exports.EJoinUpdateType || (exports.EJoinUpdateType = {}));
var EEventNotifyType;
(function (EEventNotifyType) {
    /**通知开始AI*/
    EEventNotifyType[EEventNotifyType["Notify_StartAI"] = 0] = "Notify_StartAI";
})(EEventNotifyType = exports.EEventNotifyType || (exports.EEventNotifyType = {}));
//定义AI类型
var EAIType;
(function (EAIType) {
    /*默认的类型空值*/
    EAIType[EAIType["None"] = 0] = "None";
    /**ginger 迎宾导览类型 */
    EAIType[EAIType["Ginger_Reception"] = 1] = "Ginger_Reception";
})(EAIType = exports.EAIType || (exports.EAIType = {}));
/**
 * 机器人类型
 */
class RobotType {
    static Robot_ginger = "ginger";
    static Robot_cloudginger = "cloudginger";
    static Robot_human = "human";
    static Robot_spectator = "spectator";
    // static Robot_gingerVR = "gingerVR";
    static IsRobot_ginger(robotType) {
        return robotType == this.Robot_ginger;
    }
    static IsRobot_cloudginger(robotType) {
        return robotType == this.Robot_cloudginger;
    }
    static IsRobot_human(robotType) {
        return robotType == this.Robot_human;
    }
    static IsRobot_spectator(robotType) {
        return robotType == this.Robot_spectator;
    }
}
exports.RobotType = RobotType;
//# sourceMappingURL=RobotType.js.map