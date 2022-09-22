"use strict";
/*
*  网络管理类 事件订阅 事件发送 事件派发
*/
Object.defineProperty(exports, "__esModule", { value: true });
const Logger_1 = require("Framework/Common/Logger");
/**
 * 消息管理类
 */
class MessageMgr {
    constructor() { }
    ;
    static Instance;
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new MessageMgr();
        }
        return this.Instance;
    }
    /**
     * 消息接口对象
     */
    CloundMsgDelegates = [];
    /**
     * 机器人上报消息对象
     */
    RobotMsgDelegates = [];
    /**
     * 初始化网络
     */
    Init(tsEngine) {
        Logger_1.Logger.log("[MessageMgr:Init]");
        tsEngine.EventToTSDelegate.Add(this.OnEvent.bind(this));
        tsEngine.MessageNotifyDelegate.Add(this.OnMessage.bind(this));
        //CCU swtich roc 消息
        tsEngine.OnSendRocCmd2RobotDelegate.Add(this.OnSendRocCmd2Robot.bind(this));
        tsEngine.OnSendSwitchCmd2RobotDelegate.Add(this.OnSendSwitchCmd2Robot.bind(this));
        tsEngine.OnRobotMessageDeleagte.Add(this.OnRobotMessage.bind(this));
        tsEngine.OnRobotConnectedDelegate.Add(this.OnRobotConnected.bind(this));
        tsEngine.OnRobotDisConnectedDeletage.Add(this.OnRobotDisConnected.bind(this));
        tsEngine.OnRobotInfoDeleagte.Add(this.OnRobotInfo.bind(this));
        //DS消息
        tsEngine.OnProcessDSRobotConfigMsgDelegate.Add(this.OnProcessDSRobotConfigMsg.bind(this));
        tsEngine.OnProcessDSRobotOperateMsgDelegate.Add(this.OnProcessDSRobotOperateMsg.bind(this));
        tsEngine.OnProcessDSRobotQueryMsgDelegate.Add(this.OnProcessDSRobotQueryMsg.bind(this));
    }
    UnInit(tsEngine) {
        Logger_1.Logger.log("[MessageMgr:UnInit");
        tsEngine.EventToTSDelegate.Clear();
        tsEngine.MessageNotifyDelegate.Clear();
        //CCU swtich roc 消息
        tsEngine.OnSendRocCmd2RobotDelegate.Clear();
        tsEngine.OnSendSwitchCmd2RobotDelegate.Clear();
        tsEngine.OnRobotMessageDeleagte.Clear();
        tsEngine.OnRobotConnectedDelegate.Clear();
        tsEngine.OnRobotDisConnectedDeletage.Clear();
        tsEngine.OnRobotInfoDeleagte.Clear();
        //DS消息
        tsEngine.OnProcessDSRobotConfigMsgDelegate.Clear();
        tsEngine.OnProcessDSRobotOperateMsgDelegate.Clear();
        tsEngine.OnProcessDSRobotQueryMsgDelegate.Clear();
    }
    /**
     * 注册云端消息
     * @param delegate
     * @returns
     */
    RegistCloundMsgListener(delegate) {
        this.CloundMsgDelegates.forEach(element => {
            if (element == delegate) {
                Logger_1.Logger.error("CloundMsgRegistListener faile element is regiesterd");
                return false;
            }
        });
        this.CloundMsgDelegates.push(delegate);
        return true;
    }
    /**
     * 注销云端消息
     * @param delegate
     * @returns
     */
    UnRegistCloundMsgListener(delegate) {
        let index = 0;
        this.CloundMsgDelegates.forEach(element => {
            if (element == delegate) {
                this.CloundMsgDelegates.splice(index, 1);
            }
            index += 1;
        });
        if (index == 0) {
            Logger_1.Logger.error("CloundMsgUnRegistListener faile not find Regist delegate");
        }
        //this.MessageDelegates = this.MessageDelegates.filter((value, index) => value != delegate);
    }
    /**
    * 注册机器人上报消息
    * @param delegate
    * @returns
    */
    RegistRobotMsgListener(delegate) {
        this.RobotMsgDelegates.forEach(element => {
            if (element == delegate) {
                Logger_1.Logger.error("RobotMsgRegistListener faile element is regiesterd");
                return false;
            }
        });
        this.RobotMsgDelegates.push(delegate);
        return true;
    }
    /**
     * 注销机器人上报消息
     * @param delegate
     * @returns
     */
    UnRegistRobotMsgListener(delegate) {
        let index = 0;
        this.RobotMsgDelegates.forEach(element => {
            if (element == delegate) {
                this.RobotMsgDelegates.splice(index, 1);
            }
            index += 1;
        });
        if (index == 0) {
            Logger_1.Logger.error("RobotMsgUnRegistListener faile not find Regist delegate");
        }
    }
    /**
     * 之前调用到蓝图的方法会调用到这里面来
     * @param cmd
     * @param param
     */
    OnMessage(cmd, param) {
        Logger_1.Logger.log("MessageMgr OnMessage cmd:", cmd, " param:", param);
        let len = this.CloundMsgDelegates.length;
        for (let i = 0; i < len; i++) {
            if (this.CloundMsgDelegates[i].OnMessage(cmd, param)) {
                return true;
            }
        }
        Logger_1.Logger.warn("MessageMgr没有处理消息：" + cmd, param);
        return false;
    }
    /**
     * 收到C++层事件通知 或者 通过BroadcastEvent 内部发送事件
     * @param cmd
     * @param param
     */
    OnEvent(cmd, param) {
        Logger_1.Logger.log("MessageMgr OnEvent cmd:", cmd, " param:", param);
        this.BroadcastEvent(cmd, param);
    }
    /**
     * 广播事件
     * @param cmd 事件名称
     * @param param 事件参数
     */
    BroadcastEvent(cmd, param) {
        this.CloundMsgDelegates.forEach(element => {
            element.OnEvent(cmd, param);
        });
    }
    /**来自机器的回调 */
    OnRobotInfo(robotId, robotType, robotModel) {
        Logger_1.Logger.log(`MessageMgr::OnRobotInfo robotId:${robotId},"RobotType:${robotType},robotModel:${robotModel}`);
        this.RobotMsgDelegates.forEach(element => {
            if (element.OnRobotInfo) {
                element.OnRobotInfo(robotId, robotType, robotModel);
            }
        });
    }
    OnRobotMessage(msgId, data) {
        let msgStr = "";
        /*
        if (data instanceof ArrayBuffer)
        {
            let msgLen = data.byteLength
            console.log("OnRobotMessage len:" + msgLen);
            if (msgLen > 1024 * 2)
            {
                Logger.warn("OnRobotMessage 数据大于2k 字节 可能是二进制流 不应解析为字符串 应直接转发到云端 消息ID：" + msgId);
                //转发到云端 TODO
                return;
            }

            let buf = new Uint8Array(data);

            for (var i = 0; i < buf.length; i++)
            {
                let bin = buf[i];
                msgStr + String.fromCharCode(bin);
            }
        }
        else
        */
        {
            msgStr = data;
        }
        Logger_1.Logger.log(`MessageMgr::OnRobotMessage msgId:${msgId},"data:${msgStr}`);
        this.RobotMsgDelegates.forEach(element => {
            if (element.OnRobotMessage) {
                element.OnRobotMessage(msgId, msgStr, null);
            }
        });
    }
    /**来自HARIX下发RCU通信命令 ProcessRocMsg  */
    OnSendRocCmd2Robot(cmdName, cmdJson) {
        Logger_1.Logger.log(`MessageMgr::OnSendRocCmd2Robot cmdName:${cmdName},"cmdJson:${cmdJson}`);
        this.CloundMsgDelegates.forEach(element => {
            if (element.OnSendRocCmd2Robot) {
                element.OnSendRocCmd2Robot(cmdName, cmdJson);
            }
        });
    }
    /**来自HARIX下发RCU通信命令 ProcessSwitchMsg */
    OnSendSwitchCmd2Robot(cmdName, cmdJson) {
        Logger_1.Logger.log(`MessageMgr::OnSendSwitchCmd2Robot cmdName:${cmdName},"cmdJson:${cmdJson}`);
        this.CloundMsgDelegates.forEach(element => {
            if (element.OnSendSwitchCmd2Robot) {
                element.OnSendSwitchCmd2Robot(cmdName, cmdJson);
            }
        });
    }
    OnRobotConnected() {
        Logger_1.Logger.log("MessageMgr:OnRobotConnected");
    }
    OnRobotDisConnected() {
        //Logger.log("MessageMgr:OnRobotDisConnected")
        Logger_1.Logger.LogScreen("MessageMgr:OnRobotDisConnected", undefined, 100);
    }
    OnProcessDSRobotOperateMsg(cmdName, cmdJson) {
        Logger_1.Logger.log(`MessageMgr::OnProcessDSRobotOperateMsg cmdName:${cmdName},"cmdJson:${cmdJson}`);
        this.CloundMsgDelegates.forEach(element => {
            if (element.OnProcessDSRobotOperateMsg) {
                element.OnProcessDSRobotOperateMsg(cmdName, cmdJson);
            }
        });
    }
    OnProcessDSRobotConfigMsg(cmdName, cmdJson) {
        Logger_1.Logger.log(`MessageMgr::OnProcessDSRobotConfigMsg cmdName:${cmdName},"cmdJson:${cmdJson}`);
        this.CloundMsgDelegates.forEach(element => {
            if (element.OnProcessDSRobotConfigMsg) {
                element.OnProcessDSRobotConfigMsg(cmdName, cmdJson);
            }
        });
    }
    OnProcessDSRobotQueryMsg(cmdName, cmdJson) {
        Logger_1.Logger.log(`MessageMgr::OnProcessDSRobotQueryMsg cmdName:${cmdName},"cmdJson:${cmdJson}`);
        this.CloundMsgDelegates.forEach(element => {
            if (element.OnProcessDSRobotQueryMsg) {
                element.OnProcessDSRobotQueryMsg(cmdName, cmdJson);
            }
        });
    }
}
exports.default = MessageMgr;
//# sourceMappingURL=MessageMgr.js.map