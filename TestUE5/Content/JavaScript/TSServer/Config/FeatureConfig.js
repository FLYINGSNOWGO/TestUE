"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.LanuchFirstScene = exports.FeatureConf = exports.SceneConf = exports.LanuchPersistentLevel = void 0;
const ChengDuTestScene_1 = require("GameTest/ChengDuTest/ChengDuTestScene");
const ServerGingerScene_1 = require("Scene/ServerGingerScene");
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
    // static SC_Login: iSceneConfig = { Name: "SC_Login", MapName: "ClientDefaultMap", MapPath: "/Game/Maps/Default/ClientDefaultMap", Create() { return new LoginScene(this) } };
    /**HA登录场景 */
    // static SC_HALogin: iSceneConfig = { Name: "SC_HALogin", MapName: "HALocalMap", MapPath: "/Game/Maps/HA/HALocalMap.HALocalMap", Create() { return new LoginHAScene(this) } };
    static SC_GingerScene = { Name: "SC_GingerScene", MapName: "UMap_SHZhanting", MapPath: "/Game/Maps/UMap_SHzhanting/UMap_SHZhanting.UMap_SHZhanting", Create() { return new ServerGingerScene_1.default(this); } };
    static SC_ChengDuTestScene = { Name: "SC_ChengDuTestScene", MapName: "ChengDuTest", MapPath: "/Game/Maps/Default/ChengDuTest", Create() { return new ChengDuTestScene_1.default(this); } };
}
exports.SceneConf = SceneConf;
/**
 * 逻辑功能模块定义 如果配置UIPath 则加载UI, 如果没有则是一个单纯的逻辑模块/
 */
class FeatureConf {
}
exports.FeatureConf = FeatureConf;
/**TS中的默认启动场景 */
// export let LanuchFirstScene = SceneConf.SC_RobotLogin; //SceneConf.SC_Game // //    SceneConf.SC_Game;// SceneConf.SC_Game;//
//AI测试
exports.LanuchFirstScene = SceneConf.SC_GingerScene; //SceneConf.SC_GingerScene;//SceneConf.SC_AITest; //SceneConf.SC_Game // //    SceneConf.SC_Game;// SceneConf.SC_Game;//
//# sourceMappingURL=FeatureConfig.js.map