"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Logger = void 0;
const Config_1 = require("Framework/Config");
const GameMgr_1 = require("Framework/Manager/GameMgr");
const UE = require("ue");
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
        console.log(...args);
    }
    static warn(...args) {
        if (!Config_1.default.bPrintgLogWarning)
            return;
        console.warn(...args);
    }
    static error(...args) {
        if (!Config_1.default.bPrintgLogError)
            return;
        console.error(...args);
    }
    static todo(msg) {
        msg = msg || "";
        throw new Error("Method not implemented." + msg);
    }
    /**
     * 打印消息到屏幕上
     * @param msg 消息体
     * @param color 消息颜色
     * @param dur 在屏幕上的存留时间
     */
    static LogScreen(msg, color, dur) {
        dur = dur || 3;
        color = color || new UE.LinearColor(1, 0, 0, 1);
        UE.KismetSystemLibrary.PrintString(GameMgr_1.default.GetInstance().GetWorld(), msg, true, true, color, dur);
    }
}
exports.Logger = Logger;
//# sourceMappingURL=Logger.js.map