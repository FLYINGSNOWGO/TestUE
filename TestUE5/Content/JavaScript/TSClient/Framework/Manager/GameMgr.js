"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const PlayerMgr_1 = require("./PlayerMgr");
const GameMainScene_1 = require("GameTest/Game/GameMainScene");
const Logger_1 = require("Framework/Common/Logger");
// import MessageMgr from 'Framework/Message/MessageMgr';
const puerts_1 = require("puerts");
const FeatureConfig_1 = require("Config/FeatureConfig");
const RobotAIMgr_1 = require("Framework/Manager/RobotAIMgr");
// import { SkillMgr } from 'Robot/Base/SkillMgr';
// import RobotCharacter from 'Robot/Base/RobotCharacter';
const GameMsgMgr_1 = require("Framework/Manager/GameMsgMgr");
const Delegates_1 = require("Framework/Common/Delegates");
const DataConfig_1 = require("Framework/Config/DataConfig");
/**
 * GameMgr
 *  游戏管理 场景切换 等等
 *
 */
class GameMgr //implements CloudMsgInterface
 {
    //单例模块
    static Instance;
    SceneCurrent;
    TSEngine;
    REGameInstance;
    /**UE4的角色控制器 */
    UEPlayerController;
    /**场景清理delegate */
    RemoveSceneDelegate = new Delegates_1.TSDelegate();
    /**场景初始化delegate */
    InitSceneDelegate = new Delegates_1.TSDelegate();
    //对应地图的transform
    //CCU to UE
    Scene_CUU_To_UE;
    Scene_UE_To_CCU;
    //当前的场景名字
    sCurrentSceneName = "";
    sTargetSceneName = "";
    constructor() {
    }
    ;
    /**获取单例 */
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new GameMgr();
        }
        return this.Instance;
    }
    /**重置属性数据*/
    Clean() {
        this.SceneCurrent = null;
        this.TSEngine = null;
        this.REGameInstance = null;
        this.sCurrentSceneName = "";
        this.sTargetSceneName = "";
    }
    /**初始化*/
    Init(TSEngine) {
        this.Clean();
        this.TSEngine = TSEngine;
        this.REGameInstance = TSEngine.GetGameInstance();
        this.InitMgr();
        /******上海地图配置*Begin*****/
        let aRotation = DataConfig_1.DataConfig.SH_ZhanTing_1.get("Rotation");
        let aRot = new UE.Quat(aRotation.X, aRotation.Y, aRotation.Z, aRotation.W);
        let aTranslation = DataConfig_1.DataConfig.SH_ZhanTing_1.get("Translation");
        let aLoction = new UE.Vector(aTranslation.X, aTranslation.Y, aTranslation.Z);
        let aScale = DataConfig_1.DataConfig.SH_ZhanTing_1.get("Scale");
        let Scale = new UE.Vector(aScale.X, aScale.Y, aScale.Z);
        this.Scene_CUU_To_UE = new UE.Transform(aRot, aLoction, Scale);
        //UE to CCU
        let aRotation2 = DataConfig_1.DataConfig.SH_ZhanTing_2.get("Rotation");
        let aRot2 = new UE.Quat(aRotation2.X, aRotation2.Y, aRotation2.Z, aRotation2.W);
        let aTranslation2 = DataConfig_1.DataConfig.SH_ZhanTing_2.get("Translation");
        let aLoction2 = new UE.Vector(aTranslation2.X, aTranslation2.Y, aTranslation2.Z);
        let aScale2 = DataConfig_1.DataConfig.SH_ZhanTing_2.get("Scale");
        let Scale2 = new UE.Vector(aScale2.X, aScale2.Y, aScale2.Z);
        this.Scene_UE_To_CCU = new UE.Transform(aRot2, aLoction2, Scale2);
        /******上海地图配置*End*****/
        this.OnRegistEvent();
    }
    /**注册事件和回调*/
    OnRegistEvent() {
        // MessageMgr.GetInstance().RegistCloundMsgListener(this);
        //场景加载完成的回调
        this.GetTSEngine().GameInstanceLoadCompleteDelegate.Bind(this.OnChangSceneEnd.bind(this));
        this.GetTSEngine().MapLoadBeginDelegate.Bind(this.OnMapLoadBeginDelegate.bind(this));
        this.GetTSEngine().MapLoadCompleteDelegate.Bind(this.OnGameInstanceLoadCompleteDelegate.bind(this));
    }
    /**统一的游戏退出调用接口*/
    OnExitGame() {
        this.DestroyMgr();
        PlayerMgr_1.default.GetInstance().Clean();
        //重置数据
        this.Clean();
    }
    /**初始化管理器**/
    InitMgr() {
        //创建管理器
        RobotAIMgr_1.default.CreateInstance();
        //游戏内消息的管理器
        GameMsgMgr_1.default.CreateInstance();
    }
    DestroyMgr() {
        RobotAIMgr_1.default.GetInstance().Clean();
    }
    /***获取游戏的C++对象*Begin***/
    /**
     * 获取UE4引擎游戏实例 C++对象
     * @returns
     */
    GetUEGameInstance() {
        return this.REGameInstance;
    }
    GetTSEngine() {
        return this.TSEngine;
    }
    /**
    *获取World
    */
    GetWorld() {
        return this.REGameInstance.GetWorld();
    }
    /**
     * 获取主角 Character C++对象
     * @returns
     */
    GetUECharactor(PlayerIndex) {
        return this.TSEngine.ClientCharacter;
    }
    /**
     * 获取PlayerControler C++对象
     */
    GetUEPlayerController() {
        return this.UEPlayerController;
    }
    /***获取游戏的C++对象*End***/
    /**获取鼠标位置 */
    GetMousePosition() {
        let LocationX = (0, puerts_1.$ref)(0), LocationY = (0, puerts_1.$ref)(0);
        let pc = this.GetUEPlayerController();
        let b = pc.GetMousePosition(LocationX, LocationY);
        if (b) {
            return { X: (0, puerts_1.$unref)(LocationX), Y: (0, puerts_1.$unref)(LocationY) };
        }
        else {
            return { X: 0, Y: 0 };
        }
    }
    /**
     * 运行第一个场景
     */
    RunFirstScene(firstScene) {
        let worldName = this.GetWorld().GetName();
        console.log("游戏启动运行第一个场景 firstScene.Name:", firstScene.MapName + "       worldName:" + worldName);
        if (worldName != firstScene.MapName) {
            Logger_1.Logger.log("第一个场景地图为非配置的启动场景，需要加载配置中的场景。 当前场景:" + worldName, "脚本配置场景:" + firstScene.MapName);
        }
        else {
            Logger_1.Logger.log("第一个场景地图为跟配置的启动场景一致，不需要加载配置中的场景。 当前场景:" + worldName, "脚本配置场景:" + firstScene.MapName);
        }
        this.ChangScene(firstScene, false);
    }
    /**
     * 根据当前的UE4场景切换对应的逻辑场景
     */
    ChangeSceneWitCurUE4Map() {
        let worldName = this.GetWorld().GetName();
        let sc = FeatureConfig_1.SceneConf.GetRunSceneByUE4MapName(worldName);
        if (sc) {
            this.ChangScene(sc, false);
        }
        else {
            Logger_1.Logger.error("切换场景找不到ue4场景对应的脚本场景 ue4地图名称:" + worldName);
        }
    }
    /**
     * 切换游戏场景 openlevel方式 loading界面采用AsyncLoadingScreen
     * @param conf 场景配置
     * @param loadNewMap 是否载入新地图，如果是服务器主动切换的场景，loadNewMap 传false，如果是单机模式自己切换地图，传true
     */
    ChangScene(conf, loadNewMap = true) {
        let mapPath = conf.MapPath;
        if (!conf.MapPath || conf.MapPath == "") {
            Logger_1.Logger.warn("[GameMgr:ChangScene] 切换的场景没有配置切换地图，不切换新场景，只加载脚本逻辑，场景逻辑名称:" + conf.Name);
            loadNewMap = false;
            mapPath = "";
        }
        let worldName = this.GetWorld().GetName();
        Logger_1.Logger.warn("[GameMgr:ChangScene] start, 脚本场景配置mapPath: " + mapPath + " 脚本场景配置名称: " + conf.Name, " 当前UE4地图名称:" + worldName);
        if (this.SceneCurrent) {
            /**1.通知清理当前场景 */
            this.RemoveScene();
        }
        let world = GameMgr.GetInstance().GetWorld();
        if (this.sCurrentSceneName == "") {
            this.sCurrentSceneName = world.GetName();
        }
        this.sTargetSceneName = conf.MapName;
        Logger_1.Logger.log("[GameMgr:ChangScene] ChangScene:load  " + mapPath + "  cur world Name: " + world.GetName());
        /**3.加载新场景 */
        let newScene = conf.Create();
        newScene.Init();
        if (loadNewMap) {
            Logger_1.Logger.log("[GameMgr:ChangScene] OpenLevel:" + mapPath + "  cur world:" + world.GetName());
            newScene.OpenLevel();
        }
        else {
            newScene.OnLoadLevelProgress(100);
            Logger_1.Logger.log("[GameMgr:ChangScene] not OpenLevel: " + mapPath + "  cur world:" + world.GetName());
        }
        Logger_1.Logger.log("[GameMgr:ChangScene] end:" + mapPath + "  cur world:" + world.GetName());
        /**4.更新进度 */
        let curProgesss = 0;
        let intervalHander = 0;
        let checkProgess = () => {
            if (newScene.LoadProgress >= 100) {
                clearInterval(intervalHander);
                Logger_1.Logger.log("[GameMgr:ChangScene] newScene.OnInit");
                /**初始化角色控制 */
                this.UEPlayerController = UE.GameplayStatics.GetPlayerController(this.GetWorld(), 0);
                this.SceneCurrent = newScene;
                newScene.OnInit();
                this.InitSceneDelegate.Broadcast();
                PlayerMgr_1.default.GetInstance().Init();
            }
            else {
                curProgesss += 1;
                Logger_1.Logger.log("[GameMgr:ChangScene] load level progess " + curProgesss);
            }
        };
        intervalHander = setInterval(checkProgess, 10);
    }
    /**
     * 移除场景逻辑
     */
    RemoveScene() {
        if (this.SceneCurrent) {
            this.UEPlayerController = null;
            this.RemoveSceneDelegate.Broadcast();
            PlayerMgr_1.default.GetInstance().Clean();
            RobotAIMgr_1.default.GetInstance().Clean();
            Logger_1.Logger.log("[GameMgr:RemoveScene]  清理场景 " + this.SceneCurrent.Name);
            this.SceneCurrent.OnClean();
            this.SceneCurrent.FeatureMap.forEach((key, ui) => {
                ui.RemoveFromScene();
            });
            this.SceneCurrent.Clean();
            this.SceneCurrent = null;
        }
        else {
            Logger_1.Logger.log("[GameMgr:RemoveScene]  没有正在运行的场景逻辑,不必清理");
        }
    }
    /**场景切换结束的回调接口**/
    OnChangSceneEnd(time, mapName) {
        Logger_1.Logger.log("[GameMgr:OnChangSceneEnd] mapName ", mapName, " time ", time);
        this.sCurrentSceneName = mapName;
        this.sTargetSceneName = "";
        if (this.SceneCurrent) {
        }
    }
    OnMapLoadBeginDelegate(mapth) {
        Logger_1.Logger.log("[GameMgr:OnMapLoadBeginDelegate]");
        this.RemoveScene();
    }
    OnGameInstanceLoadCompleteDelegate(loadTime, mapName) {
        Logger_1.Logger.log("[GameMgr:OnGameInstanceLoadCompleteDelegate]");
    }
    /**
     * 获取当前场景
     * @returns
     */
    static GetCurScene() {
        return GameMgr.GetInstance().SceneCurrent;
    }
    /**获取当前游戏场景 */
    static GetCurGameScene() {
        let sc = GameMgr.GetInstance().SceneCurrent;
        if (sc instanceof GameMainScene_1.default) {
            return sc;
        }
        return null;
    }
    static GetUIByName(uiName) {
        return GameMgr.GetCurScene().GetFeatureByName(uiName);
    }
    /**回登录界面*/
    ReturnLogin() {
        this.ChangScene(FeatureConfig_1.SceneConf.SC_Login);
    }
}
exports.default = GameMgr;
//# sourceMappingURL=GameMgr.js.map