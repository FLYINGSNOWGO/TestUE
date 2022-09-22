"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
console.log("------------------------------Server START TSProServer/TSServer/Main.ts------------------------------");
const Logger_1 = require("Framework/Common/Logger");
const puerts_1 = require("puerts");
let tsEngine = puerts_1.argv.getByName("TSEngine");
//开启断点调试等待，调用此函数调用之后需要启动 Run->Start Debugging(F5) 否则游戏一启动，此文件已经加载过了，无法断点此文件代码//
// tsEngine.SetDebugWaiting();
const Test_1 = require("Test");
const UE = require("ue");
//import HotUpdate from 'HotUpdate/HotUpdate';
/**测试 */
Test_1.default.Run2();
/**是否是专用服务器 */
let IsDedicatedServer = UE.KismetSystemLibrary.IsDedicatedServer(tsEngine.GetGameInstance().GetWorld());
console.log("--ServerTag-- tsEngine name =  " + tsEngine.GetName(), "IsDedicatedServer:", IsDedicatedServer);
let IsServer = UE.KismetSystemLibrary.IsServer(tsEngine.GetGameInstance().GetWorld());
console.log("--ServerTag-- tsEngine name =  " + tsEngine.GetName(), "IsServer:", IsServer);
let IsStandalone = UE.KismetSystemLibrary.IsStandalone(tsEngine.GetGameInstance().GetWorld());
console.log("--ServerTag-- tsEngine name =  " + tsEngine.GetName(), "IsStandalone:", IsStandalone);
/**进入游戏逻辑 */
function EnterGame() {
    if (bEnterGame) {
        Logger_1.Logger.warn("[Main:setTimeout] 已经启动了TS游戏逻辑,请勿再次启动!");
        return;
    }
    bEnterGame = true;
    //因为是gameintance 启动的，需要延迟一帧，否在地图还没加载角色，初始化角色数据失败
    setTimeout(() => {
        Logger_1.Logger.log("[Main:setTimeout]------------------------------进入游戏------------------------------");
        // let gameStart = require("GameStart.js");
        // let start = gameStart.default.GetInstance();
        // start.StartGame(IsDedicatedServer, IsServer, IsStandalone);
    }, 10);
}
/**是否已经进入游戏了 */
let bEnterGame = false;
/**Windows平台暂时没有更新 */
let platFrom = UE.GameplayStatics.GetPlatformName();
console.log(`--ServerTag--[Main.ts]------------------------------系统类型:${platFrom}------------------------------`);
if (platFrom == "Windows" || platFrom == "Linux") {
    console.log(`--ServerTag--[Main.ts]------------------------------未更新 进入游戏------------------------------`);
    EnterGame();
}
else {
    // tsEngine.EventToTSDelegate.Add((EventCmd: UE.CLIENT_EVENT2TS, Params: string) =>
    // {
    //     if (EventCmd == UE.CLIENT_EVENT2TS.EVENT_UPDATE_COMPLETE)
    //     {
    //         console.log(`--ServerTag--[Main.ts]------------------------------更新完成 进入游戏------------------------------`);
    //         EnterGame();
    //     }
    // })
}
/*
let hotUpdate = new HotUpdate();
hotUpdate.Init(gameInstance);
hotUpdate.CheckUpdate();
hotUpdate.UpdateaFinishCallback = () =>
{
   EnterGame();
}
*/
//# sourceMappingURL=Main.js.map