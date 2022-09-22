"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.LanuchFirstScene = exports.FeatureConf = exports.SceneConf = exports.LanuchPersistentLevel = void 0;
const LoginScene_1 = require("GameTest/Login/LoginScene");
const LoginUI_1 = require("GameTest/Login/LoginUI");
const GameMainUI_1 = require("GameTest/Game/GameMainUI");
const GameMainScene_1 = require("GameTest/Game/GameMainScene");
const RobotLoginScene_1 = require("Robot/Base/RobotLoginScene");
const GingerTestUI_1 = require("Robot/GingerRobot/GingerTestUI");
const GingeTestScene_1 = require("Robot/GingerRobot/GingeTestScene");
const LoginHAScene_1 = require("HAClient/LoginHAScene");
const LoginHAUI_1 = require("HAClient/LoginHAUI");
const GingerRobotMainScene_1 = require("Robot/GingerRobot/GingerRobotMainScene");
const POIMgr_1 = require("HAClient/HA/POIMgr");
const EmulationUI_1 = require("Robot/Base/EmulationUI");
const GingeNavigationTestScene_1 = require("GameTest/NavigationObstacleTest/GingeNavigationTestScene");
const ChengDuTestMainScene_1 = require("GameTest/chengDuTest/ChengDuTestMainScene");
const SmartVoiceUI_1 = require("Robot/GingerRobot/SmartVoiceUI");
const Logger_1 = require("Framework/Common/Logger");
/**
 * 配置接口定义
 *
 */
/**游戏启动地图 */
exports.LanuchPersistentLevel = "/Game/Maps/Default/ClientDefaultMap";
/**
 * 场景配置定义
 *
 */
class SceneConf {
    /**登录场景 */
    //static SC_Login: iSceneConfig = { Name: "ClientDefaultMap", LevelName: "/Game/Maps/Default/ClientDefaultMap", CreateInstance() { return new LoginScene(this) } };
    static SC_Login = { Name: "SC_Login", MapName: "ClientDefaultMap", MapPath: "/Game/Maps/Default/ClientDefaultMap", Create() { return new LoginScene_1.default(this); } };
    /**HA登录场景 */
    static SC_HALogin = { Name: "SC_HALogin", MapName: "HALocalMap", MapPath: "/Game/Maps/HA/HALocalMap.HALocalMap", Create() { return new LoginHAScene_1.default(this); } };
    /**主游戏场景 */
    static SC_Game = { Name: "SC_Game", MapName: "scene01_01", MapPath: "/Game/Res/Map/scene/scene01_01?game=/Game/Res/GameMode/Game_BP_mode.Game_BP_mode_C", Create() { return new GameMainScene_1.default(this); } };
    /**机器人登录场景，目前用于所有机器人 */
    static SC_RobotLogin = { Name: "SC_RobotLogin", MapName: "ClientDefaultMap", MapPath: "/Game/Maps/Default/ClientDefaultMap", Create() { return new LoginScene_1.default(this); } };
    /**连接服务器后 Ginger机器人场景*/
    static SC_ChengDuTestScene_test = { Name: "SC_ChengDuTestScene_test", MapName: "ChengDuTest", MapPath: "", Create() { return new ChengDuTestMainScene_1.default(this); } };
    static SC_ChengDuTestScene = { Name: "SC_ChengDuTestScene", MapName: "UMap_ChengDu", MapPath: "", Create() { return new ChengDuTestMainScene_1.default(this); } };
    static SC_GingerMainScene = { Name: "SC_GingerScene", MapName: "UMap_SHzhanting", MapPath: "/Game/Maps/UMap_SHzhanting/UMap_SHzhanting.UMap_SHzhanting", Create() { return new GingerRobotMainScene_1.default(this); } };
    /**Ginger测试场景 */
    //static SC_GingerTestScene: ISceneConfig = { Name: "SC_GingerTestScene", MapName: "NewMap", MapPath: "/Game/Maps/MapTest/NewMap", Create() { return new GingerTestScene(this) } };
    static SC_GingerTestScene = { Name: "SC_GingerTestScene", MapName: "ClientDefaultMap", MapPath: "/Game/Maps/Default/ClientDefaultMap", Create() { return new GingeTestScene_1.default(this); } };
    /**寻路测试场景 */
    static SC_NavigationTestScene = { Name: "SC_NavigationTestScene", MapName: "NavigationTestMap", MapPath: "/Game/Maps/NavigatiionTestMap/NavigationTestMap", Create() { return new GingeNavigationTestScene_1.default(this); } };
    /**寻路测试场景 */
    static SC_SmartTestScene = { Name: "SC_SmartTestScene", MapName: "NewMap", MapPath: "/Game/Maps/MapTest/NewMap", Create() { return new GingeTestScene_1.default(this); } };
    /**机器人登录场景 */
    static SC_RobotLoginScene = { Name: "SC_RobotLoginScene", MapName: "ClientDefaultMap", MapPath: "/Game/Maps/Default/ClientDefaultMap", Create() { return new RobotLoginScene_1.default(this); } };
    /**根据UE4场景名称获取对应的脚本模块 */
    static GetRunSceneByUE4MapName(ueMapName) {
        let sc = null;
        if (ueMapName == this.SC_ChengDuTestScene.MapName) {
            sc = this.SC_ChengDuTestScene;
        }
        else if (ueMapName == this.SC_ChengDuTestScene_test.MapName) {
            sc = this.SC_ChengDuTestScene_test;
        }
        else if (ueMapName == this.SC_GingerMainScene.MapName) {
            sc = this.SC_GingerMainScene;
        }
        else if (ueMapName == this.SC_RobotLoginScene.MapName) {
            sc = this.SC_RobotLoginScene;
        }
        else if (ueMapName == this.SC_NavigationTestScene.MapName) {
            sc = this.SC_NavigationTestScene;
        }
        else {
            sc = null;
        }
        if (!sc) {
            Logger_1.Logger.error("GetRunSceneByUE4MapName 找不到 UE4地图名称:" + ueMapName + " 的脚本场景");
        }
        else {
            Logger_1.Logger.warn("GetRunSceneByUE4MapName 找到 UE4地图名称:" + ueMapName + " 的脚本场景,名称:" + sc.Name);
        }
        return sc;
    }
}
exports.SceneConf = SceneConf;
/**
 * 逻辑功能模块定义 如果配置UIPath 则加载UI, 如果没有则是一个单纯的逻辑模块/
 */
class FeatureConf {
    /**测试Ginger功能 */
    static UI_GingerTest = { Name: "UI_GingerTest", UIName: "TestUI", Zorder: 0, UIPath: "/Game/UI/TestUI.TestUI_C", Create(uiData) { return new GingerTestUI_1.default(this, uiData); } };
    /**测试Ginger功能 */
    static UI_Emulation = { Name: "UI_Emulation", UIName: "EmulationUI", Zorder: 0, UIPath: "/Game/UI/Emulation/EmulationUI.EmulationUI_C", Create(uiData) { return new EmulationUI_1.default(this, uiData); } };
    /**登录 */
    static UI_Login = { Name: "UI_Login", UIName: "", Zorder: 0, UIPath: null, Create(uiData) { return new LoginUI_1.default(this, uiData); } };
    /**HA登录 */
    static UI_HALogin = { Name: "UI_HALogin", Zorder: 0, UIPath: "/Game/UI/HA/HAWebUI.HAWebUI_C", Create(uiData) { return new LoginHAUI_1.default(this, uiData); } };
    /**HA登录 */
    static Mgr_POI = { Name: "Mgr_POI", Zorder: 0, UIPath: null, Create(uiData) { return new POIMgr_1.default(this, uiData); } };
    /**测试纯逻辑模块 无UI */
    static UI_Login2 = { Name: "UI_Login2", UIName: "", Zorder: 0, UIPath: null, Create(uiData) { return new LoginUI_1.default(this, uiData); } };
    /**游戏主界面 */
    static UI_GameMain = { Name: "UI_GameMain", UIName: "GameUI", Zorder: 0, UIPath: "/Game/Res/UI/Game/GameUI.GameUI_C", Create(uiData) { return new GameMainUI_1.default(this, uiData); } };
    /**PC端语音界面 */
    static UI_SmartVoic = { Name: "UI_SmartVoic", UIName: "UI_SmartVoice", Zorder: 0, UIPath: "WidgetBlueprint'/Game/UI/UI_SmartVoice.UI_SmartVoice_C'", Create(uiData) { return new SmartVoiceUI_1.default(this, uiData); } };
}
exports.FeatureConf = FeatureConf;
/**TS中的默认启动场景 */
// export let LanuchFirstScene = SceneConf.SC_RobotLogin; //SceneConf.SC_Game // //    SceneConf.SC_Game;// SceneConf.SC_Game;//
//AI测试
exports.LanuchFirstScene = SceneConf.SC_RobotLoginScene; //SC_NavigationTestScene //SC_RobotLoginScene//SC_ChengDuTestScene; ////SC_RobotLoginScene; //;//SceneConf.SC_GingerMainScene; //
//# sourceMappingURL=FeatureConfig.js.map