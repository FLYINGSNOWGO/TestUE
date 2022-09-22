"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
const GameMgr_1 = require("./GameMgr");
const ActivityMgr_1 = require("./ActivityMgr");
const Logger_1 = require("Framework/Common/Logger");
/*
*  主角管理类 角色生成 角色数据管理
*/
class PlayerMgr {
    PlayerCamera;
    constructor() { }
    static Instance;
    /**当前角色*自己*/
    MyCharacter;
    /**角色控制器 */
    ArrivalPlayerController;
    /**默认相机臂的旋转角度*/
    DefaultCamerArmInputRotation;
    /**屏幕大小 */
    ScreenSize;
    // /**只是用于记录当前**/
    Local_RobotID;
    Local_RobotType;
    ResetData() {
        this.MyCharacter = null;
        this.Local_RobotID = "";
        this.Local_RobotType = "";
        this.ArrivalPlayerController = null;
    }
    /**
     * 获取单例
     */
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new PlayerMgr();
        }
        return this.Instance;
    }
    /**场景调用初始化 */
    Init() {
        ActivityMgr_1.default.GetInstance().AddGameTickListener(this.Update, this);
    }
    /**清理数据 停止update*/
    Clean() {
        Logger_1.Logger.log("[PlayerMgr:Clean] do ");
        this.ResetData();
        ActivityMgr_1.default.GetInstance().RemoveGameTickListener(this.Update, this);
    }
    /**设置当前实体机器人角色 */
    SetMyCharacter(character) {
        this.MyCharacter = character;
    }
    /**获取当前实体机器人角色 */
    GetMyCharacter() {
        return this.MyCharacter;
    }
    /** 获取PlayerControler C++对象**/
    GetUEPlayerController() {
        return this.ArrivalPlayerController;
    }
    /** 获取当前角色的gameAI */
    // GetPlayerGameAI()
    // {
    //     if (this.GameCharacter)
    //     {
    //         return this.GameCharacter.GetGameAI()
    //     }
    //     return null;
    // }
    /**存储当前本地自身的类型和ID**/
    InitMyTypeID(RobotParam) {
        let RobotSplit = RobotParam.split("|");
        this.Local_RobotType = RobotSplit[0];
        this.Local_RobotID = RobotSplit[1];
    }
    GetLocalRobotID() {
        return this.Local_RobotID;
    }
    /**初始化角色 */
    InitCharacter(character) {
        if (!character) {
            console.error("初始化角色失败 请传入UE4角色 ");
            return;
        }
        this.SetMyCharacter(character);
        /**Controller的赋值**/
        this.ArrivalPlayerController = GameMgr_1.default.GetInstance().GetUEPlayerController();
        this.ArrivalPlayerController.SetVirtualJoystickVisibility(true);
        let screenX = (0, puerts_1.$ref)(undefined);
        let screenY = (0, puerts_1.$ref)(undefined);
        this.ArrivalPlayerController.GetViewportSize(screenX, screenY);
        this.ScreenSize = new UE.Vector2D((0, puerts_1.$unref)(screenX), (0, puerts_1.$unref)(screenY));
        this.PlayerCamera = this.MyCharacter.GetUEActor().GetComponentByClass(UE.CameraComponent.StaticClass());
        return this.MyCharacter;
    }
    UpdateDis_OneSecond = 1;
    Update(dt) {
        //每秒更新
        if (this.UpdateDis_OneSecond > 0) {
            this.UpdateDis_OneSecond -= dt;
            if (this.UpdateDis_OneSecond <= 0) {
                this.UpdateDis_OneSecond = 1;
                this.UpdateOneSecond(this.UpdateDis_OneSecond);
            }
        }
    }
    /**是否是当前正在控制角色 */
    IsPlayerControlCharactor(character) {
        return this.MyCharacter.GetRobotID() == character.GetRobotID();
    }
    /**每秒更新 */
    UpdateOneSecond(dt) {
    }
}
exports.default = PlayerMgr;
//# sourceMappingURL=PlayerMgr.js.map