"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = void 0;
const Singleton_1 = require("Framework/Base/Singleton");
const Logger_1 = require("Framework/Common/Logger");
const GameMgr_1 = require("Framework/Manager/GameMgr");
// import GingerCharacter from "Robot/GingerRobot/GingerCharacter";
// let GingerCharacter;
/**
 * 玩家列表管理器
 */
class RobotAIMgr extends Singleton_1.default {
    static Instance = null;
    /**
    * 机器人列表
    */
    RobotList;
    static CreateInstance() {
        if (!this.Instance) {
            this.Instance = new RobotAIMgr();
            this.Instance.Init();
        }
        return this.Instance;
    }
    // static GetInstance(): RobotAIMgr
    // {
    //     return this.Instance;
    // }
    /**重置数据*/
    ResetData() {
        if (this.RobotList) {
            for (let aCharacter of this.RobotList.values()) {
                //执行角色的移除
                // aCharacter.DestroyActor();
                aCharacter.Clean();
            }
            this.RobotList.clear();
        }
        else {
            this.RobotList = new Map();
        }
    }
    Init() {
        this.ResetData();
        let tsEngine = GameMgr_1.default.GetInstance().GetTSEngine();
        if (tsEngine) {
            tsEngine.OnActorBeginPlayToTSDeleagte.Add(this.OnUEActerBeginPlay.bind(this));
            tsEngine.OnActorEndPlayToTSDeleagte.Add(this.OnUEActerEndPlay.bind(this));
        }
    }
    Clean() {
        this.ResetData();
    }
    /**
     * Actor Begin Play
     * @param actor
     */
    OnUEActerBeginPlay(actor) {
        let robot = actor;
        Logger_1.Logger.log(`[RobotAIMgr:OnUEActerBeginPlay] Robot Info: robotId:${robot.robotId}; type: ${robot.robotType} `);
        if (robot.robotId) {
            let aRobot = this.CreateRobot(robot);
            //添加进引用
            this.AddRobotCharacter(aRobot);
        }
    }
    /**
     * Actor End Play
     * @param actor
     */
    OnUEActerEndPlay(actor) {
        let robot = actor;
        if (robot.robotId) {
            this.RomoveRobot(robot.robotId);
        }
    }
    /**
     * 增加机器对象
     * @param char
     */
    AddRobotCharacter(char) {
        // let robot = char.GetUEActor() as UE.RobotEngineCharacter;
        // let robotId = robot.robotId;
        let robotId = char.GetRobotID();
        Logger_1.Logger.log("[RobotAIMgr:AddRobotCharacter] add robotcharacter robotid: ", robotId);
        if (robotId && robotId != "" && !this.RobotList.has(robotId)) {
            this.RobotList[robotId] = char;
        }
        else {
            Logger_1.Logger.error("[RobotAIMgr:AddRobotCharacter] Error 机器人ID 错误 ");
        }
    }
    /**
     * 获取机器人
     * @param robotID
     */
    GetRobotCharacter(robotID) {
        return this.RobotList[robotID];
    }
    /**
     * 创建一个机器人逻辑对象
     * @param robot  C++的character对象
     */
    CreateRobot(robot) {
        Logger_1.Logger.log(`[RobotAIMgr:CreateRobot] robotType: ${robot.robotType}`);
        if (robot.robotType == 'ginger') {
            let GingerCharacter = require("Robot/GingerRobot/GingerCharacter").GingerCharacter;
            //创建Ginger机器人对象 主角
            Logger_1.Logger.log("[RobotAIMgr:CreateRobot] create robot ginger", robot);
            let ginger = GingerCharacter.Create(GingerCharacter, robot);
            return ginger;
        }
        return null;
    }
    /**
     * 移除 机器人
     * @param robotID
     */
    RomoveRobot(robotID) {
        let aRobot = this.RobotList.get(robotID);
        if (aRobot) {
            //这里需要讨论是否执行脚本的移除 *****
            // aRobot.Clean();
            this.RobotList.delete(robotID);
        }
    }
    /**判断当前是否已经有对应的机器人
     *
     * **/
    GetIsHasRobot(robotID) {
        return this.RobotList.has(robotID);
    }
}
exports.default = RobotAIMgr;
//# sourceMappingURL=RobotAIMgr.js.map