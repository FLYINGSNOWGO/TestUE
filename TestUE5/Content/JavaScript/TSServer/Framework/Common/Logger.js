"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Logger = void 0;
const Config_1 = require("Config/Config");
var LogType;
(function (LogType) {
    LogType[LogType["Error"] = 0] = "Error";
    LogType[LogType["Assert"] = 1] = "Assert";
    LogType[LogType["Warning"] = 2] = "Warning";
    LogType[LogType["Log"] = 3] = "Log";
    LogType[LogType["Exception"] = 4] = "Exception";
})(LogType || (LogType = {}));
let isEditor = false; // 判断是否是处于编辑器模式
class Logger {
    static getPrintStack(type, showStack, ...args) {
        let message = '';
        for (let i = 0; i < args.length; i++) {
            const element = args[i];
            if (typeof element === 'object') {
                message += JSON.stringify(element);
            }
            else {
                message += element;
            }
            if (i < args.length - 1) {
                message += ' ';
            }
        }
        if (showStack || isEditor) {
            var stacks = new Error().stack.split('\n');
            for (let i = 3; i < stacks.length; i++) {
                const line = stacks[i];
                message += '\n';
                message += line;
            }
        }
        return message;
    }
    static log(...args) {
        if (!Config_1.default.bPrintgLogDebug)
            return;
        console.log("---Server---", ...args);
    }
    static warn(...args) {
        if (!Config_1.default.bPrintgLogWarning)
            return;
        console.warn("---Server---", ...args);
    }
    static error(...args) {
        if (!Config_1.default.bPrintgLogError)
            return;
        console.error("---Server---", ...args);
    }
    static todo(msg) {
        msg = msg || "";
        throw new Error("---Server---" + " Method not implemented." + msg);
    }
}
exports.Logger = Logger;
//# sourceMappingURL=Logger.js.map