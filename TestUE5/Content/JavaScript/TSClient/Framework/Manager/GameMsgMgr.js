"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const Logger_1 = require("Framework/Common/Logger");
const GameMgr_1 = require("Framework/Manager/GameMgr");
const MessageMgr_1 = require("Framework/Message/MessageMgr");
const RobotAIMgr_1 = require("./RobotAIMgr");
const PlayerMgr_1 = require("./PlayerMgr");
/**
 * 玩家列表管理器
 */
class GameMsgMgr {
    static Instance = null;
    static CreateInstance() {
        if (!this.Instance) {
            this.Instance = new GameMsgMgr();
            this.Instance.Init();
        }
        return this.Instance;
    }
    static GetInstance() {
        return this.Instance;
    }
    /**重置数据*/
    ResetData() {
    }
    Init() {
        this.ResetData();
        this.OnRegistEvent();
    }
    Clean() {
        this.ResetData();
        this.OnUnRegistEvent();
    }
    /**注册事件和回调*/
    OnRegistEvent() {
        MessageMgr_1.default.GetInstance().RegistCloundMsgListener(this);
    }
    /**注册事件和回调*/
    OnUnRegistEvent() {
        MessageMgr_1.default.GetInstance().UnRegistCloundMsgListener(this);
    }
    /*****角色相关消息的转发*Begin*****/
    OnMessage(cmd, param) {
        // throw new Error("Method not implemented.");
        return false;
    }
    OnEvent(cmd, param) {
        // throw new Error("Method not implemented.");
        Logger_1.Logger.log("[GameMgr::OnEvent] onEvent cmd " + cmd + " param " + param);
        //收到服务器连接成功 并地图加载完成
        if (cmd == UE.CLIENT_EVENT2TS.EVENT_InitLocalCharacter) {
            // let RobotSplit = param.split("|")
            // this.Local_RobotID = RobotSplit[0]
            // this.Local_RobotType = RobotSplit[1]
            PlayerMgr_1.default.GetInstance().InitMyTypeID(param);
            let name = GameMgr_1.default.GetInstance().GetWorld().GetName();
            Logger_1.Logger.log("[GameMgr::OnEvent] To Create Local Character ", name);
            GameMgr_1.default.GetInstance().ChangeSceneWitCurUE4Map();
        }
        else if (cmd == UE.CLIENT_EVENT2TS.EVENT_ClientGameAINotify) {
            Logger_1.Logger.log("[GingerRobotMainScene::OnEvent] Get DS param ", param);
            let aJsonObj = JSON.parse(param);
            if (aJsonObj) {
                //查找对应的机器人角色来触发技能
                let aRobot = RobotAIMgr_1.default.GetInstance().GetRobotCharacter(aJsonObj.RobotID);
                if (aRobot) {
                    aRobot.OnReceiveDSJson(aJsonObj);
                }
            }
            // //传入对应的角色做解析
            // let aGingerCharacter = PlayerMgr.GetInstance().GetMyCharacter() as GingerCharacter;
            // if (aGingerCharacter)
            // {
            //     aGingerCharacter.OnReceiveDSJson(param);
            // }
        }
    }
    OnSendRocCmd2Robot(cmdName, cmdJson) {
        //throw new Error("Method not implemented.");
    }
    OnSendSwitchCmd2Robot(cmdName, cmdJson) {
        //throw new Error("Method not implemented.");
    }
    OnProcessDSRobotOperateMsg(cmdName, cmdJson) {
        //throw new Error("Method not implemented.");
    }
    OnProcessDSRobotConfigMsg(cmdName, cmdJson) {
        //throw new Error("Method not implemented.");
    }
    OnProcessDSRobotQueryMsg(cmdName, cmdJson) {
        //throw new Error("Method not implemented.");
    }
}
exports.default = GameMsgMgr;
//# sourceMappingURL=GameMsgMgr.js.map