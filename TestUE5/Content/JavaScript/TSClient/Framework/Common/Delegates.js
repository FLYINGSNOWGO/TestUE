"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.TSDelegate = void 0;
/**
 * 类似UE4的多播代理实现
 */
class TSDelegate {
    Events = new Array();
    ExistValue(fn) {
        for (let fn1 of this.Events) {
            if (fn1[0] == fn[0] && fn1[1] == fn[1]) {
                return true;
            }
        }
        return false;
    }
    //监听事件
    Add(obj, fn) {
        let value = [obj, fn];
        let bExist = this.ExistValue(value);
        if (!bExist) {
            this.Events.push(value);
        }
    }
    //发送事件
    Broadcast(...args) {
        this.Events.forEach(element => {
            let fun = element[1];
            let obj = element[0];
            fun.apply(obj, args);
        });
    }
    //移除一个事件监听
    Remove(obj, fn) {
        let value = [obj, fn];
        let bExist = this.ExistValue(value); //this.Events.findIndex(fn);
        let index = -1;
        for (let fn1 of this.Events) {
            index += 1;
            if (value == fn1) {
                break;
            }
        }
        if (index != -1) {
            // delete this.Events[index]; //此种删除方式 会导致 this.Events[index]=undefined;
            this.Events.splice(index, 1);
        }
    }
    //移除一个key事件的所有监听
    Clear() {
        this.Events = [];
    }
}
exports.TSDelegate = TSDelegate;
//# sourceMappingURL=Delegates.js.map