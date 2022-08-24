import Config from "Config/Config";
// import GameMgr from "Framework/Manager/GameMgr";
import * as UE from "ue";


enum LogType
{
    Error = 0,
    Assert = 1,
    Warning = 2,
    Log = 3,
    Exception = 4
}

let isEditor = false  // 判断是否是处于编辑器模式

export class Logger
{

    private static getPrintStack(type: LogType, showStack: boolean, ...args)
    {
        let message = '';
        for (let i = 0; i < args.length; i++)
        {
            const element = args[i];
            if (typeof element === 'object')
            {
                message += JSON.stringify(element);
            } else
            {
                message += element;
            }
            if (i < args.length - 1)
            {
                message += ' ';
            }
        }

        if (showStack || isEditor)
        {
            var stacks = new Error().stack.split('\n');
            for (let i = 3; i < stacks.length; i++)
            {
                const line = stacks[i];
                message += '\n';
                message += line;
            }
        }


        return message;
    }


    static log(...args): void
    {
        if (!Config.bPrintgLogDebug) return;
        console.log("---Server---", ...args);
    }


    static warn(...args): void
    {
        if (!Config.bPrintgLogWarning) return;
        console.warn("---Server---", ...args);
    }


    static error(...args): void
    {
        if (!Config.bPrintgLogError) return;
        console.error("---Server---", ...args);
    }

    static todo(msg?: string)
    {
        msg = msg || "";
        throw new Error("---Server---" + " Method not implemented." + msg);
    }

    /**
     * 打印消息到屏幕上
     * @param msg 消息体
     * @param color 消息颜色
     * @param dur 在屏幕上的存留时间
     */
    // static LogScreen(msg: string, color?: UE.LinearColor, dur?: number)
    // {
    //     dur = dur || 3;
    //     color = color || new UE.LinearColor(1, 0, 0, 1);
    //     UE.KismetSystemLibrary.PrintString(GameMgr.GetInstance().GetWorld(), msg, true, true, color, dur);
    // }
}