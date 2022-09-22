"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const GameMgr_1 = require("../Manager/GameMgr");
const Logger_1 = require("Framework/Common/Logger");
const puerts_1 = require("puerts");
/**
 * 场景基类 一个游戏场景逻辑需要继承此类
 */
class BaseScene {
    /**关卡名字 UE4中的LevelName */
    LevelName;
    /**TS逻辑名称 */
    Name;
    /**所拥有的UI */
    FeatureMap;
    /**初始化进度，请在需要的地方填写进度值 0-100*/
    LoadProgress;
    // Func Map 
    FuncMap = new Map();
    //场景中的ActorArry
    ActorArry = (0, puerts_1.$ref)();
    LevelLoadCallback;
    constructor(conf) {
        this.LevelName = conf.MapPath;
        this.Name = conf.Name;
        this.FeatureMap = new Map();
        this.LoadProgress = 0;
    }
    //初始化地图，其中有UE载入地图
    Init() {
        //GameMgr.GetInstance().GetTSEngine()
    }
    /**载入地图 异步加载 */
    LoadLevel(callback) {
        /*
        console.log("LoadLevel " + this.LevelName);
        let world = GameMgr.GetWorld();

        this.LevelLoadCallback = callback;
        //目前采用流方式，才能实现异步加载
        GameMgr.GetGameInstance().LoadLevelStream(world, this.LevelName, true, false);
        GameMgr.GetGameInstance().LevelStreamLoadCompleteDelegate.Unbind();
        GameMgr.GetGameInstance().LevelStreamLoadCompleteDelegate.Bind((percent: number) =>
        {
            GameMgr.GetGameInstance().LevelStreamLoadCompleteDelegate.Unbind();
            this.OnLoadLevelProgress(percent);
        });
        */
    }
    /**以直接打开关卡的方式  PIE模式看不到loadng界面，只有非PIE模式可以看到得到 */
    OpenLevel(callback) {
        GameMgr_1.default.GetInstance().GetTSEngine().GameInstanceLoadCompleteDelegate.Bind((time, mapName) => {
            GameMgr_1.default.GetInstance().GetTSEngine().GameInstanceLoadCompleteDelegate.Unbind();
            let world2 = GameMgr_1.default.GetInstance().GetWorld();
            let inName = world2.GetName();
            console.log("BaseScene:GameInstanceLoadCompleteDelegate: " + mapName + " " + inName);
            this.OnLoadLevelProgress(100);
        });
        console.log("[BaseScene:OpenLevel] LevelName " + this.LevelName);
        let world = GameMgr_1.default.GetInstance().GetWorld();
        //UE.GameplayStatics.OpenLevel(world, this.LevelName);
        this.LevelLoadCallback = callback;
    }
    /**
     * 场景加载完成
     * @param progress 目前没啥用 加载完成会传一个"100"， 后面可能用来传入进度
     * @returns
     */
    OnLoadLevelProgress(progress) {
        console.log("BaseScene LoadLevelComplete: " + this.LevelName);
        this.LoadProgress = 100;
        if (this.LevelLoadCallback) {
            this.LevelLoadCallback.call(this);
            this.LevelLoadCallback = null;
        }
        this.OnLoadLevelComplete();
        return true;
    }
    /**设置地图加载完成回调 */
    SetLevelLoadCompleteCallback(callback) {
        this.LevelLoadCallback = callback;
    }
    /**添加一个逻辑逻辑模块
     * @name 逻辑模块模块名称
     * @uifile UMG蓝图文件 可以不传就是一个纯粹的功能逻辑
    */
    AddFeature(conf, uiData) {
        let feature = conf.Create(uiData);
        this.FeatureMap.set(feature, conf.Name);
        feature.Init(this);
        feature.OnInit();
        return feature;
    }
    /**从场景中移除UI逻辑模块
     * @ui ui逻辑模块
     */
    RemoveFeature(feature) {
        feature.RemoveFromScene();
    }
    /**
     * 移除UI 逻辑
     */
    RemoveFeatureFromArr(feature) {
        if (this.FeatureMap.has(feature)) {
            this.FeatureMap.delete(feature);
        }
        else {
            console.error("ERROR:RemoveUIFromMap not find ui " + feature.Name);
        }
    }
    /**
     * 获取UI
     */
    GetFeatureByName(uiName) {
        // 使用对象解析
        for (let [key, value] of this.FeatureMap) {
            if (value == uiName) {
                return key;
            }
        }
    }
    /**
     * 场景清理，场景卸载完成后调用
     */
    Clean() {
        this.FeatureMap.clear();
        this.LevelName = null;
        this.Name = null;
    }
    /**
     *  设置player0角色到默认出生点   切换场景完成会调用，防止角色掉下去
     */
    SetDefautPlayerStart() {
        let world = GameMgr_1.default.GetInstance().GetWorld();
        let gamestart = UE.GameplayStatics.GetActorOfClass(world, UE.PlayerStart.StaticClass());
        let player0 = UE.GameplayStatics.GetPlayerCharacter(world, 0);
        if (player0 && gamestart) {
            player0.K2_SetActorLocation(gamestart.K2_GetActorLocation(), false, null, false);
            player0.K2_SetActorRotation(gamestart.K2_GetActorRotation(), false);
            let pc = player0.Controller;
            pc.SetViewTargetWithBlend(player0);
            //pc.Possess(pp);
        }
    }
    //统一调用PlayerMgr接口
    // /**提供一个获取当前场景主角色的的接口，方便使用 */
    //GetPlayerCharacter(): BaseCharacter
    //{
    //     return PlayerMgr.GetInstance().GetMyCharacter();
    // }
    /************************************************************逻辑接口**************************************************/
    /**地图载入完成 */
    OnLoadLevelComplete() {
        console.log("BaseScene OnLoadLevelComplete" + this.Name);
    }
    /**收到初始化消息 场景加载完成有会调用此接口
     *
    */
    OnInit() {
        Logger_1.Logger.log("[BaseScene:OnInit] " + this.Name);
    }
    /**收到清理消息，销毁时候会调用
     *
     */
    OnClean() {
        this.FuncMap.clear();
        Logger_1.Logger.log("[BaseScene:OnClean] " + this.Name);
    }
    ProcessInterfaceEvent(Name, Data, Callback) {
        let JsonStr = UE.JsonLibraryHelpers.JsonValue_Stringify(Data, false);
        Logger_1.Logger.log(`[BaseScene] ProcessInterfaceEvent:${Name} ${JsonStr},Callback:${Callback}`);
        let DataObj = JSON.parse(JsonStr);
        let Func = Name;
        let TheFunc = this.FuncMap.get(Func);
        if (!TheFunc) {
            Logger_1.Logger.warn("[BaseScene:ProcessInterfaceEvent] : Not registered the Func: ", Func);
        }
        else {
            TheFunc(DataObj, Callback);
            Logger_1.Logger.log("[BaseScene:ProcessInterfaceEvent] Func:", Func);
        }
        //UE.WebInterfaceHelpers.WebInterfaceCallback_Call($ref<UE.WebInterfaceCallback>(Callback), UE.JsonLibraryHelpers.ConstructNull());
    }
    // GetWorld()
    // {
    //     return GameMgr.GetInstance().GetWorld();
    // }
    /**
     * 获取场景中某类型的Actor
     * @param ActorClass
     * @returns
     */
    GetAllActor(ActorClass) {
        UE.GameplayStatics.GetAllActorsOfClass(GameMgr_1.default.GetInstance().GetWorld(), ActorClass, this.ActorArry);
        return (0, puerts_1.$unref)(this.ActorArry);
    }
}
exports.default = BaseScene;
//# sourceMappingURL=BaseScene.js.map