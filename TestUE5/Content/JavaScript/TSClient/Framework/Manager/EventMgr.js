"use strict";
/*
*  事件管理类 事件订阅 事件发送 事件派发
*/
Object.defineProperty(exports, "__esModule", { value: true });
class EventMgr //extends Singleton
 {
    constructor() { }
    ;
    static Instance;
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new EventMgr();
            this.Instance.Init();
        }
        return this.Instance;
    }
    Events;
    //初始化
    Init() {
        this.Events = {};
    }
    //监听事件
    static AddEventListener(key, callbackFun, obj) {
        let callbacks = this.GetInstance().Events[key] || [];
        callbacks && callbacks.push([callbackFun, obj]);
        this.GetInstance().Events[key] = callbacks;
    }
    //发送事件
    static SendEvent(key, ...args) {
        let callbacks = this.GetInstance().Events[key] || [];
        callbacks.forEach(fn => fn[0].call(fn[1], args));
    }
    //移除一个事件监听
    static RemoveListener(key, callbackFun, obj) {
        let callbacks = this.GetInstance().Events[key] || [];
        let retCallbacks = callbacks.filter(fn => fn[0] !== callbackFun || fn[1] != obj);
        this.GetInstance().Events[key] = retCallbacks;
    }
    //移除一个key事件的所有监听
    static RemoveAllListener(key) {
        this.GetInstance().Events[key] = [];
    }
}
exports.default = EventMgr;
//# sourceMappingURL=EventMgr.js.map