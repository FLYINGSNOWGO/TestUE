"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
console.log("------------------------------GameStart.ts loading------------------------------");
const GameMgr_1 = require("Framework/Manager/GameMgr");
const puerts_1 = require("puerts");
const MessageMgr_1 = require("Framework/Message/MessageMgr");
const Logger_1 = require("Framework/Common/Logger");
const FeatureConfig_1 = require("Config/FeatureConfig");
/**游戏开始类，用于初始化有些数据等 初始化加载界面  更新初始化等 todo */
class GameStart {
    constructor() {
        console.log("GameStart constructor");
    }
    static Instance;
    GameIsStart = false;
    // TSEngine: UE.TSClientEngine;
    //获取当前的运行模式
    IsDedicatedServer = false; //是否是专用服务器
    IsServer = false;
    IsStandalone = false;
    /**
     * 获取单例
     */
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new GameStart();
        }
        return this.Instance;
    }
    ResetData() {
        this.IsDedicatedServer = false;
        this.IsServer = false;
        this.IsStandalone = false;
        this.GameIsStart = false;
    }
    //初始化
    Init(world, TsEngine) {
        this.ResetData();
        TsEngine.GameShutDownDelegate.Add(this.ShutDownGame.bind(this));
        console.log("GameStart Init : " + world.GetName());
    }
    IsInServer() {
        return this.IsDedicatedServer; //|| this.IsServer;
    }
    IsInClient() {
        return this.IsStandalone;
    }
    //启动游戏
    StartGame(IsDedicatedServer, IsServer, IsStandalone) {
        console.log("------------------------------GameStart------------------------------");
        let tsEngine = puerts_1.argv.getByName("TSEngine");
        // this.TSEngine = tsEngine;
        //保存标识
        this.IsDedicatedServer = IsDedicatedServer;
        this.IsServer = IsServer;
        this.IsStandalone = IsStandalone;
        this.GameIsStart = true;
        let instance = tsEngine.GetGameInstance(); // UE.GameplayStatics.GetGameInstance(tsEngine);
        let world = instance.GetWorld();
        Logger_1.Logger.log("[GameStart:StartGame] IsDedicatedServer: ", this.IsDedicatedServer);
        this.Init(world, tsEngine);
        console.log("[GameStart:StartGame] tsEngine UniqueID ", tsEngine.GetUniqueID());
        MessageMgr_1.default.GetInstance().Init(tsEngine);
        GameMgr_1.default.GetInstance().Init(tsEngine);
        let worldName = world.GetName();
        let firstScene = FeatureConfig_1.SceneConf.GetRunSceneByUE4MapName(worldName);
        console.log("游戏启动，初始化的第一个场景配置 firstScene.MapName:" + firstScene.MapName + "    firstScene.MapPath:" + firstScene.MapPath);
        GameMgr_1.default.GetInstance().RunFirstScene(firstScene);
    }
    GetGameIsStart() {
        return this.GameIsStart;
    }
    //游戏退出
    ShutDownGame() {
        this.GameIsStart = false;
        let tsEngine = GameMgr_1.default.GetInstance().GetTSEngine();
        if (tsEngine) {
            MessageMgr_1.default.GetInstance().UnInit(tsEngine);
            tsEngine.EngineTickDelegate.Clear();
            tsEngine.EventToTSDelegate.Clear();
            tsEngine.MessageNotifyDelegate.Clear();
            tsEngine.OnSendSwitchCmd2RobotDelegate.Clear();
            tsEngine.GameTickDelegate.Clear();
            tsEngine.EventToTSDelegate.Clear();
            tsEngine.MapLoadBeginDelegate.Unbind();
            tsEngine = null;
        }
        GameMgr_1.default.GetInstance().OnExitGame();
        console.log("[GameStart:ShutDownGame] 游戏退出");
    }
}
exports.default = GameStart;
//# sourceMappingURL=GameStart.js.map