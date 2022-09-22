"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.SkillBase = exports.ESkilltate = void 0;
const Logger_1 = require("Framework/Common/Logger");
const ActivityMgr_1 = require("Framework/Manager/ActivityMgr");
const FunTool_1 = require("Framework/Common/FunTool");
var ESkilltate;
(function (ESkilltate) {
    /**未初始化 */
    ESkilltate[ESkilltate["Uninitialized"] = 0] = "Uninitialized";
    /**暂停状态 */
    ESkilltate[ESkilltate["Paused"] = 1] = "Paused";
    /**执行中 */
    ESkilltate[ESkilltate["Runing"] = 2] = "Runing";
    /**已经结束 */
    ESkilltate[ESkilltate["Finished"] = 3] = "Finished";
    /**非正常的强制结束***/
    ESkilltate[ESkilltate["ForStop"] = 4] = "ForStop";
})(ESkilltate = exports.ESkilltate || (exports.ESkilltate = {}));
;
class SkillBase {
    RandTimeOutHandle;
    /**Skill优先级 */
    Priority = 0;
    /**任务状态 */
    SkillState = ESkilltate.Uninitialized;
    /**是否可以暂停 */
    bCanPause = false;
    /**是否可以结束任务 */
    bCanFinish = true;
    /**任务执行者 */
    // Character: BaseCharacter;
    RobotID;
    /**开启tick */
    TickEnable = false;
    /**创建时候传入的参数 */
    InitParams;
    /**技能管理器 */
    SkillMgr;
    /**技能结束回调 */
    FinishCallBack = (skill) => { };
    FuncMap = new Map();
    /**设置一个定时结束此任务时间，在启动此技能之后 */
    FinishTime = 0;
    FinishCDTimeHandle;
    /**技能定义信息 */
    SkillInfo;
    Guid; //From Server 
    IdxID = null;
    constructor(Guid, taskDefine, initParams) {
        let str = JSON.stringify(initParams);
        this.SkillInfo = taskDefine;
        this.InitParams = initParams.skillparam;
        this.Guid = Guid;
        this.IdxID = initParams.skillidxid;
        this.RobotID = initParams.robotid;
        this.Priority = this.SkillInfo.Priority;
    }
    /**获取技能ID 全局唯一，由创建时候生成 */
    GetGuid() {
        return this.Guid;
    }
    /**获取任务名称 */
    GetSkillName() {
        return this.SkillInfo.SkillName;
    }
    ResetData() {
        this.SkillMgr = null;
    }
    Clean() {
        this.SkillMgr = null;
    }
    /**初始化 */
    Init(skillMgr) {
        this.SkillMgr = skillMgr;
        this.OnInit();
        this.FuncMap.set("RPC", this.RPCNotify.bind(this));
        this.FuncMap.set("STA", this.StartRun.bind(this));
        this.FuncMap.set("FIN", this.Finish.bind(this));
        this.FuncMap.set("Pause", this.Pause.bind(this));
        this.FuncMap.set("Recover", this.Recover.bind(this));
    }
    /**设置结束回调 */
    SetFinishCallBack(finishCallback) {
        this.FinishCallBack = finishCallback;
    }
    /**任务开始 */
    StartRun() {
        Logger_1.Logger.LogScreen("任务开始 SkillBase StartRun：" + this.GetSkillName());
        this.OnStartRun();
        this.SkillState = ESkilltate.Runing;
        if (this.FinishTime > 0) {
            this.FinishCDTimeHandle = setTimeout(() => {
                this.Finish(true);
            }, this.FinishTime * 1000);
        }
        this.NotifySkillMgrSkillStart();
    }
    /* Notify RPC Call */
    RPCNotify(initParams) {
    }
    /* Notify RPC Call */
    /**通知管理器技能开始 */
    NotifySkillMgrSkillStart() {
        if (this.SkillMgr) {
            this.SkillMgr.OnSkillStart(this);
        }
    }
    /**
     * 结束运行 会在管理器中移除此技能
     * @param bSuccess 是否执行成功
    */
    Finish(bSuccess) {
        if (!this.CanFinish()) {
            return false;
        }
        this.OnFinish(bSuccess);
        if (this.FinishCallBack) {
            this.FinishCallBack(this);
        }
        this.SkillState = ESkilltate.Finished;
        this.NotifySkillMgrSkillFinish();
        Logger_1.Logger.log("任务结束 TaskBase Finish ：" + this.GetSkillName());
        this.StopRandTime();
    }
    NotifySkillMgrSkillFinish() {
        if (this.SkillMgr) {
            this.SkillMgr.OnSkillFinish(this);
        }
    }
    /**强制的移除技能
     *
     * **/
    ForceStopRun() {
        if (!this.IsRuning()) {
            Logger_1.Logger.log("[SkillBase:ForceStopRun] skill cur not run");
            this.SkillState = ESkilltate.ForStop;
            this.StopRandTime();
            return;
        }
        this.SkillState = ESkilltate.ForStop;
        this.OnFinish(false);
        if (this.FinishCallBack) {
            this.FinishCallBack(this);
        }
    }
    /**开启tick  */
    SetTickEnable(b) {
        if (b) {
            if (!this.TickEnable) {
                ActivityMgr_1.default.GetInstance().AddGameTickListener(this.Tick, this);
                this.TickEnable = true;
            }
        }
        else {
            if (this.TickEnable) {
                ActivityMgr_1.default.GetInstance().RemoveGameTickListener(this.Tick, this);
                this.TickEnable = false;
            }
        }
    }
    /**设置结束时间 秒 在Active之后开始计时 */
    SetFinishTime(sec) {
        this.FinishTime = sec;
    }
    /**tick */
    Tick(dt) {
        this.OnTick(dt);
    }
    /** 是否可以暂停*/
    CanPause() {
        return this.bCanPause;
    }
    /**
     * 设置是否可以暂停
     * @param b
     */
    SetCanPause(b) {
        this.bCanPause = b;
    }
    /**是否可以结束 */
    CanFinish() {
        return true;
    }
    Pause() {
        this.SkillState = ESkilltate.Paused;
        this.OnPause();
        this.NotifySkillMgrSkillPause();
    }
    NotifySkillMgrSkillPause() {
        if (this.SkillMgr) {
            this.SkillMgr.OnSKillPause(this);
        }
    }
    /**恢复暂停 外部调用的，不重写 */
    Recover() {
        if (this.IsPaused()) {
            this.SkillState = ESkilltate.Runing;
            this.OnRecover();
            this.NotifySkillMgrSkillRecover();
        }
    }
    NotifySkillMgrSkillRecover() {
        if (this.SkillMgr) {
            this.SkillMgr.OnSkillRecover(this);
        }
    }
    IsRuning() {
        return (this.SkillState == ESkilltate.Runing);
    }
    IsPaused() {
        return (this.SkillState == ESkilltate.Paused);
    }
    IsFinished() {
        return (this.SkillState == ESkilltate.Finished) || this.SkillState == ESkilltate.ForStop;
    }
    /**设置随机时间调用OnRandTime函数 秒为单位  */
    StartRandomTime(min, max) {
        if (this.RandTimeOutHandle) {
            clearTimeout(this.RandTimeOutHandle);
        }
        this.RandTimeOutHandle = setTimeout(() => {
            this.OnRandTime();
        }, FunTool_1.default.GetRandomNum(min, max) * 1000);
    }
    /**清除随机时间调用 */
    StopRandTime() {
        if (this.RandTimeOutHandle) {
            clearTimeout(this.RandTimeOutHandle);
            this.RandTimeOutHandle = null;
        }
    }
    /*************************************子类应继承接口********************************** */
    /**初始化 */
    OnInit() {
    }
    /**消息事件*/
    OnMessage(cmd, param) {
        return false;
    }
    /**收到事件 */
    OnEvent(cmd, param) {
    }
    /**CCU 事件 */
    OnRobotMessage(msgId, data, dataLength) {
        return false;
    }
    /**
     * 收到暂停
     * @returns
     */
    OnPause() {
        return true;
    }
    /**
     * tick
     * @param dt
     */
    OnTick(dt) {
    }
    /**
     * 激活调用
     */
    OnStartRun() {
    }
    /**
     * 收到结束
     */
    OnFinish(bSuccess) {
    }
    /**
     * 收到暂停恢复
     */
    OnRecover() {
    }
    /**随机时间到了 由this.StartRandomTime 开启 */
    OnRandTime() {
    }
}
exports.SkillBase = SkillBase;
//# sourceMappingURL=SkillBase.js.map