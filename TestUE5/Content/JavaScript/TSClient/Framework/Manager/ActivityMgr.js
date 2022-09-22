"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const ue_1 = require("ue");
/**
 * 游戏生命周期接口：tick接口，游戏退出接口
 */
class GameActivityMgr {
    /**单例模式 */
    constructor() { }
    ;
    static Instance;
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new GameActivityMgr();
            this.Instance.Init();
        }
        return this.Instance;
    }
    /**tick 列表 */
    GameTickListenerArr;
    /**GameShutdown 列表 */
    GameShoudownListenerArr;
    Init() {
        this.GameTickListenerArr = new Array();
        this.GameShoudownListenerArr = new Array();
        ue_1.TSClientEngine.GetTSInstance().GameTickDelegate.Add(this.GameTick.bind(this));
        ue_1.TSClientEngine.GetTSInstance().GameShutDownDelegate.Add(this.GameShutDown.bind(this));
    }
    /**
     * 引擎tick调用
     */
    GameTick(dt) {
        if (this.GameTickListenerArr) {
            this.GameTickListenerArr.forEach(fn => fn[0].call(fn[1], dt));
        }
    }
    /**
     * 游戏停止调用
     */
    GameShutDown() {
        console.log(" 游戏停止调用 ");
        if (this.GameShoudownListenerArr) {
            this.GameShoudownListenerArr.forEach(fn => fn[0].call(fn[1]));
        }
    }
    /**
     * 引擎tick
     * @param callback 监听函数
     * @returns
     */
    AddGameTickListener(callback, obj) {
        //let val = ;
        //let index = this.EngineTickListenerArr.indexOf([callback, obj]);
        let len = this.GameTickListenerArr.length;
        for (let i = 0; i < len; i++) {
            if (this.GameTickListenerArr[i][0] == callback && this.GameTickListenerArr[i][1] == obj) {
                console.warn("GameActivityMgr AddEngineTickListener is exit function " + obj.name + ":", callback);
                return;
            }
        }
        console.log("AddEngineTickListener " + callback.name);
        this.GameTickListenerArr.push([callback, obj]);
    }
    /**
     * 移除引擎tick
     * @param callback
     */
    RemoveGameTickListener(callback, obj) {
        function ispositive(fn, index, array) {
            let b = (fn[0] == callback && fn[1] == obj);
            return !b;
        }
        this.GameTickListenerArr = this.GameTickListenerArr.filter(ispositive);
    }
    /**
     * 添加游戏关闭监听
     * @param callback
     */
    AddGameShutDownListener(callback, obj) {
        let len = this.GameShoudownListenerArr.length;
        for (let i = 0; i < len; i++) {
            if (this.GameShoudownListenerArr[i][0] == callback && this.GameShoudownListenerArr[i][1] == obj) {
                console.warn("GameActivityMgr AddGameShutDownListener is exit function ", obj.name + ":", callback);
                return;
            }
        }
        this.GameShoudownListenerArr.push([callback, obj]);
    }
    /**
     * 移除游戏关闭监听
     * @param callback
     */
    RemoveGameShutDownListner(callback, obj) {
        this.GameTickListenerArr = this.GameTickListenerArr.filter(fn => fn[0] !== callback && fn[1] != obj);
    }
}
exports.default = GameActivityMgr;
//# sourceMappingURL=ActivityMgr.js.map