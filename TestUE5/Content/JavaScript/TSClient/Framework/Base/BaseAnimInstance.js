"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
require("Framework/Base/ObjectExt");
const Logger_1 = require("Framework/Common/Logger");
const ActivityMgr_1 = require("Framework/Manager/ActivityMgr");
const Delegates_1 = require("Framework/Common/Delegates");
/***对于动画实例的封装，不直接创建实例****/
class BaseAnimInstance {
    MaxMoveSpeed;
    /**对应的TS对象***/
    TSOwner;
    /** 蓝图函数更新句柄  */
    UpdateAnimationHandle;
    /**动作事件回调 */
    AnimNotifyDelegate = new Delegates_1.TSDelegate();
    /**蒙太奇动画结束回调 */
    MontageEndDelegate = new Delegates_1.TSDelegate();
    /**角色的动画实例 C++对象**/
    AnimInstance; //;
    MoveSpeed;
    constructor() {
        Logger_1.Logger.log("[BaseAnimInstance:constructor] do here");
    }
    /**重置数据***/
    ResetData() {
        this.MaxMoveSpeed = 600;
        this.TSOwner = null;
        this.AnimInstance = null;
    }
    Init(aTSOwner, Ins) {
        Logger_1.Logger.log("[BaseAnimInstance:Init] do here");
        this.ResetData();
        this.AnimInstance = Ins;
        this.TSOwner = aTSOwner;
        /**初始化动作回调事件事件 */
        let Character = this.AnimInstance.TryGetPawnOwner();
        if (Character) {
            Logger_1.Logger.log("[BaseAnimInstance:Init] do Bind");
            this.AnimInstance.NotifyDelegate.Bind((name, eventParam) => {
                this.OnAnimNotify(name, eventParam);
            });
            this.AnimInstance.OnMontageEnded.Add(this.OnMontageEnd.bind(this));
        }
    }
    /**对象清理***/
    Clean() {
        this.ResetData();
    }
    /**
     * 设置UpdateAnimation开启关闭
     * @param b 是否开启Update
     * @returns
     */
    SetUpdateAnimationEnable(b) {
        if (b) {
            if (this.UpdateAnimationHandle) {
                return;
            }
            ActivityMgr_1.default.GetInstance().AddGameTickListener(this.UpdateAnimation, this);
        }
        else {
            if (this.UpdateAnimationHandle) {
                clearInterval(this.UpdateAnimationHandle);
                this.UpdateAnimationHandle = null;
            }
            ActivityMgr_1.default.GetInstance().RemoveGameTickListener(this.UpdateAnimation, this);
        }
    }
    /**
     * 基类继承此函数再里面实现状态更新
     * @param DeltaTimeX
     */
    UpdateAnimation(DeltaTimeX) {
    }
    OnAnimNotify(eventName, eventParam) {
        Logger_1.Logger.log("[BaseAnimInstance:OnAnimNotify] do eventName ", eventName, " eventParam ", eventParam);
        if (this.AnimNotifyDelegate) {
            this.AnimNotifyDelegate.Broadcast(eventName, eventParam);
        }
    }
    OnMontageEnd(Montage, bInterrupted) {
        this.MontageEndDelegate.Broadcast(Montage, bInterrupted);
    }
    SetFloatValue(key, val) {
        this.AnimInstance.SetFloatValue(key, val);
    }
    SetBoolValue(key, val) {
        this.AnimInstance.SetBoolValue(key, val);
    }
    SetIntValue(key, val) {
        return this.AnimInstance.SetIntValue(key, val);
    }
    SetStringValue(key, val) {
        return this, this.AnimInstance.SetStringValue(key, val);
    }
    GetFloatValue(key) {
        return this.AnimInstance.GetFloatValue(key);
    }
    GetBoolValue(key) {
        return this.AnimInstance.GetBoolValue(key);
    }
    GetIntValue(key) {
        return this.AnimInstance.GetIntValue(key);
    }
    GetStingValue(key) {
        return this.AnimInstance.GetStringValue(key);
    }
    /**设置移动速度 */
    SetMoveSpeed(speed) {
        this.AnimInstance.MoveSpeed = speed;
    }
    GetMoveSpeed() {
        return this.AnimInstance.MoveSpeed;
    }
    //设置yaw
    SetYaw(Yaw) {
        this.AnimInstance.Yaw = Yaw;
    }
}
exports.default = BaseAnimInstance;
//# sourceMappingURL=BaseAnimInstance.js.map