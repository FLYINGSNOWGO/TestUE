"use strict";
/**
 *  游戏数据模块
 */
Object.defineProperty(exports, "__esModule", { value: true });
exports.EJsonFuncType = exports.ETaskNodeState = exports.EAIType = exports.EEntityTag = void 0;
/**
 * 实体类型，场景中所有需要有交互的对象可称为实体
 *
 */
var EEntityTag;
(function (EEntityTag) {
    /**无类型 */
    EEntityTag[EEntityTag["None"] = 0] = "None";
    /**主角*/
    EEntityTag[EEntityTag["RobotCharacter"] = 1] = "RobotCharacter";
})(EEntityTag = exports.EEntityTag || (exports.EEntityTag = {}));
/*******Ai相关的枚举**Begin*****/
//定义AI类型
var EAIType;
(function (EAIType) {
    /*默认的类型空值*/
    EAIType[EAIType["None"] = 0] = "None";
    /**ginger 迎宾导览类型 */
    EAIType[EAIType["Ginger_Reception"] = 1] = "Ginger_Reception";
})(EAIType = exports.EAIType || (exports.EAIType = {}));
//当前行为节点的状态
var ETaskNodeState;
(function (ETaskNodeState) {
    ETaskNodeState[ETaskNodeState["None"] = 0] = "None";
    ETaskNodeState[ETaskNodeState["State_Execute"] = 1] = "State_Execute";
    ETaskNodeState[ETaskNodeState["State_FinishExecute"] = 2] = "State_FinishExecute";
    ETaskNodeState[ETaskNodeState["State_Abort"] = 3] = "State_Abort";
    ETaskNodeState[ETaskNodeState["State_FinishAbort"] = 4] = "State_FinishAbort";
})(ETaskNodeState = exports.ETaskNodeState || (exports.ETaskNodeState = {}));
/**Json传递时，所对应的功能枚举**/
var EJsonFuncType;
(function (EJsonFuncType) {
    /**行为节点的通知*Begin**/
    EJsonFuncType[EJsonFuncType["TaskNode_Execute_Type"] = 0] = "TaskNode_Execute_Type";
    EJsonFuncType[EJsonFuncType["TaskNode_Abort_Type"] = 1] = "TaskNode_Abort_Type";
    EJsonFuncType[EJsonFuncType["TaskNode_Tick_Type"] = 2] = "TaskNode_Tick_Type";
    /**行为节点的通知*End**/
    /**传递属性或者赋值*Begin*/
    EJsonFuncType[EJsonFuncType["TaskState_Update"] = 3] = "TaskState_Update";
    EJsonFuncType[EJsonFuncType["AnimAttr_Update"] = 4] = "AnimAttr_Update";
    EJsonFuncType[EJsonFuncType["JoinState_Upate"] = 5] = "JoinState_Upate";
    EJsonFuncType[EJsonFuncType["CustomEvent_Notify"] = 6] = "CustomEvent_Notify";
    /**传递属性或者赋值*End*/
})(EJsonFuncType = exports.EJsonFuncType || (exports.EJsonFuncType = {}));
// ///行为树节点传递的技能大类
// export enum ESkillBigType
// {
// }
/*******Ai相关的枚举**End*****/
//# sourceMappingURL=DataType.js.map