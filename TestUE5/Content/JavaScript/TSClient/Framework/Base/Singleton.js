"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
/*
*  单例模式 继承此类
*/
class Singleton {
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new this();
        }
        return this.Instance;
    }
}
exports.default = Singleton;
//# sourceMappingURL=Singleton.js.map