"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = void 0;
/**
 * 角色相关基类
 */
const UE = require("ue");
const EntityMgr_1 = require("Framework/Manager/EntityMgr");
const AssetMgr_1 = require("Framework/Manager/AssetMgr");
const GameMgr_1 = require("Framework/Manager/GameMgr");
const BaseAnimInstance_1 = require("./BaseAnimInstance");
const ActivityMgr_1 = require("Framework/Manager/ActivityMgr");
const BaseActor_1 = require("./BaseActor");
const Logger_1 = require("Framework/Common/Logger");
const FunTool_1 = require("Framework/Common/FunTool");
/**
 * UE4角色基类封装
 */
class BaseCharacter extends BaseActor_1.default {
    /**默认碰撞预设类型 */
    DefalutCollosionObjectType;
    /**是否在游戏中隐藏的 */
    HideInGame = true;
    /**对pawn的默认碰撞回应类型 */
    DefalutCollisionResponseToPawn;
    /**默认朝向 */
    RotationRateDefault;
    /**默认跟运动开启状态 */
    RootMotionDefault;
    /**出生点 */
    BornPos;
    /**技能管理器 */
    SKillsMgr;
    /**UE4 角色 */
    UECharacter;
    // UECharacter: UE.RobotEngineCharacter;
    /**角色动画实例 TS对象 */
    BaseAniminstance; //BaseAnimInstance
    /**输入事件绑定map */
    InputMap;
    MoveSpeedDefault;
    /**动画事件回调 */
    OnAnimNotifyCallback;
    /**碰撞事件回调 */
    OnActorHitCallback;
    /**外部的注册函数代理  */
    FuncMap = new Map();
    // constructor()
    // {
    //     super();
    //     // this.InputMap = new Map<Function, UE.InputBind>();
    // }
    /**初始化设置*/
    Init(character) {
        Logger_1.Logger.log("[BaseCharacter:Init] do here");
        super.Init(character);
        this.UECharacter = this.GetUEActor();
        this.HideInGame = this.UECharacter.bHidden;
        this.InputMap.clear();
        this.InitDefaultValue();
        this.BaseAniminstance = null;
        character.EndPlayDelegate.Unbind();
        character.EndPlayDelegate.Bind(this.EndPlay.bind(this));
        Logger_1.Logger.warn("RegeisterEntity " + this.GetUObjectUniqueID());
        //初始化动作实例
        this.InitAnimInstance();
    }
    ResetData() {
        if (this.InputMap) {
            this.InputMap.forEach((value, key) => {
                value.InputeDelegateBindAction.Unbind();
                value.InputeDelegateBindAxis.Unbind();
                value.InputeDelegateBindTouch.Unbind();
            });
            this.InputMap.clear();
        }
        else {
            this.InputMap = new Map();
        }
        this.HideInGame = true;
        if (this.BaseAniminstance) {
            this.BaseAniminstance.Clean();
        }
        this.BaseAniminstance = null;
        if (this.SKillsMgr) {
            this.SKillsMgr.Clean();
        }
        this.SKillsMgr = null;
        // Logger.warn("UnRegeisterEntity " + this.GetUEActorName());
        this.UECharacter = null;
        //最后再移除父类数据
        super.ResetData();
    }
    /**初始化默认值 */
    InitDefaultValue() {
        let CharacterMovement = this.UECharacter.CharacterMovement;
        let ani = this.UECharacter.Mesh.GetAnimInstance();
        if (CharacterMovement) {
            this.MoveSpeedDefault = this.UECharacter.CharacterMovement.MaxWalkSpeed;
            this.RotationRateDefault = this.UECharacter.CharacterMovement.RotationRate.Yaw;
        }
        if (ani) {
            this.RootMotionDefault = this.UECharacter.Mesh.GetAnimInstance().RootMotionMode;
        }
        this.BornPos = this.UECharacter.K2_GetActorLocation();
        Logger_1.Logger.warn("出生点位置 " + this.BornPos.ToString());
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            let CharacterMovement = aCharacter.CharacterMovement;
            if (CharacterMovement) {
                this.MoveSpeedDefault = aCharacter.CharacterMovement.MaxWalkSpeed;
                this.RotationRateDefault = aCharacter.CharacterMovement.RotationRate.Yaw;
            }
            let ani = aCharacter.Mesh.GetAnimInstance();
            if (ani) {
                this.RootMotionDefault = aCharacter.Mesh.GetAnimInstance().RootMotionMode;
            }
            this.BornPos = aCharacter.K2_GetActorLocation();
            Logger_1.Logger.warn("[BaseCharacter:InitDefaultValue] BornPos " + this.BornPos.ToString());
            this.DefalutCollosionObjectType = this.UECharacter.CapsuleComponent.GetCollisionObjectType();
            this.DefalutCollisionResponseToPawn = this.UECharacter.CapsuleComponent.GetCollisionResponseToChannel(UE.ECollisionChannel.ECC_Pawn);
            this.UECharacter.CapsuleComponent.OnComponentHit.Add((HitComponent, OtherActor, OtherComp, NormalImpulse, Hit) => {
                if (this.OnActorHitCallback) {
                    this.OnActorHitCallback(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
                }
                this.OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
            });
        }
    }
    /**初始化动作的TS实例，父类可以直接覆盖**/
    InitAnimInstance() {
        let ani = new BaseAnimInstance_1.default();
        ani.Init(this, this.UECharacter.Mesh.GetAnimInstance());
        this.SetTsAnimInstance(ani);
    }
    /**
     * 设置动画处理实例逻辑类
     * @param ins BaseAnimInstance子类对象
     */
    SetTsAnimInstance(ins) {
        if (this.BaseAniminstance) {
            this.BaseAniminstance.Clean();
        }
        this.BaseAniminstance = ins;
    }
    /**
     * 获取动画实例逻辑对象
     * @returns
     */
    GetTsAnimInstance() {
        return this.BaseAniminstance;
    }
    /**
     * 获取UE4动画实例对象
     * @returns
     */
    GetUEAnimInstance() {
        return this.UECharacter.Mesh.GetAnimInstance();
    }
    /** AACharacter BeginPlay() 只有在TS中新创建角色才可调用到此函数*/
    BeginPlay() {
        console.warn("[BaseCharacter:BeginPlay] 引擎主动 创建对象===============>> " + this.GetUEActorName());
    }
    /** 收到UE Actor 中的调用  AACharacter EndPlay() */
    EndPlay() {
        if (this.UECharacter) {
            Logger_1.Logger.warn("[BaseCharacter:EndPlay]引擎主动 销毁对象===============>> " + this.UECharacter.GetName());
        }
        this.Clean();
    }
    Clean() {
        this.SetUpdateEnable(false);
        this.CleanKeyPointsSplineMove();
        //清除数据
        super.Clean();
    }
    /**绑定输入事件 角色基本输入事件已经在C++ 绑定，每帧都调用的事件(如BindAxis) 最好不要在这里绑定 影响效率. inputEventType 为 BindAxis类型时bindEvent可以不用传*/
    BindInputEvent(inputEventType, eventKey, callback, bindEvent) {
        if (this.InputMap.has(callback)) {
            Logger_1.Logger.error("BindInputEvent is exit");
            return;
        }
        if (!(this.UECharacter && this.UECharacter.InputComponent)) {
            Logger_1.Logger.warn(" this.ArrivalCharacter cannot BindInputEvent ");
            return;
        }
        let bindIput = new UE.InputBind();
        let inputCom = this.UECharacter.InputComponent;
        bindIput.BindAction(inputCom, inputEventType, eventKey, bindEvent);
        switch (inputEventType) {
            case UE.EInputBindType.BindAction:
                bindIput.InputeDelegateBindAction.Bind(() => {
                    callback.call(this);
                });
                break;
            case UE.EInputBindType.BindAxis:
                bindIput.InputeDelegateBindAxis.Bind((dt) => {
                    callback.call(this, dt);
                });
                break;
            case UE.EInputBindType.BindTouch:
                bindIput.InputeDelegateBindTouch.Bind((dt) => {
                    callback.call(this, dt);
                });
                break;
            default:
                {
                    Logger_1.Logger.warn("BindInputEvent bind event error inputEventType: " + inputEventType);
                }
                break;
        }
        this.InputMap.set(callback, bindIput);
    }
    /**
     * 设置转身速度
     * @param Angle 角度 每秒的角度
     */
    SetRotationRate(Angle) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.RotationRate.Yaw = Angle;
        }
    }
    /**设置默认转身速度*/
    SetDefaultRotationRate() {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.RotationRate.Yaw = this.RotationRateDefault;
        }
    }
    /**
     * 设置最大移动速度
     */
    SetMaxSpeed(sp) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.MaxWalkSpeed = sp;
        }
    }
    /**
     * 设置默认速度
     */
    SetDefaultSpeed() {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.MaxWalkSpeed = this.MoveSpeedDefault;
        }
        else {
            Logger_1.Logger.error("[BaseCharacter:SetDefaultSpeed] not Character");
        }
    }
    /**设置移动速度缩放 */
    SetSpeedScale(scale) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            let curspeed = aCharacter.CharacterMovement.MaxWalkSpeed;
            aCharacter.CharacterMovement.MaxWalkSpeed = curspeed * scale;
        }
        else {
            Logger_1.Logger.error("[BaseCharacter:SetSpeedScale] not Character");
        }
    }
    /**飞行模式 */
    SetFlyMode() {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.SetMovementMode(UE.EMovementMode.MOVE_Flying);
        }
        else {
            Logger_1.Logger.error("[BaseCharacter:SetFlyMode] not Character");
        }
    }
    /**走路模式 */
    SetWalkMode() {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.SetMovementMode(UE.EMovementMode.MOVE_Walking);
        }
        else {
            Logger_1.Logger.error("[BaseCharacter:SetFlyMode] not Character");
        }
    }
    /**设置在跟运动中可以旋转控制 playercontroler 旋转控制 */
    SetAllowPhysicsRotationDurRootMotion(allowRot) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            aCharacter.CharacterMovement.bAllowPhysicsRotationDuringAnimRootMotion = allowRot;
        }
    }
    /**查找最近的实体 */
    GetNearestEntityActor(dis) {
        let aCharacter = this.GetUEActor();
        if (!aCharacter) {
            Logger_1.Logger.error("[BaseCharacter:GetNearestEntityActor] not Character");
            return;
        }
        // let actor = aCharacter;
        let localCation = aCharacter.K2_GetActorLocation();
        let ActorsToIgnore = GameMgr_1.default.GetInstance().GetTSEngine().TempArr;
        ActorsToIgnore.Empty();
        ActorsToIgnore.Add(aCharacter);
        let hitRsult = UE.TSHelper.GetRecentlyActorByTrace(aCharacter, localCation, localCation, dis, UE.ETraceTypeQuery.TraceTypeQuery3, false, ActorsToIgnore, UE.EDrawDebugTrace.None, UE.ECheckTraceType.ObjectChannel);
        let nearestActor = null;
        let nearestDis = Number.MAX_VALUE;
        if (hitRsult.Num() > 0) {
            for (let i = 0; i < hitRsult.Num(); i++) {
                let hitActor = hitRsult.Get(i).HitObjectHandle.Actor;
                let entity = EntityMgr_1.default.GetEntity(hitActor.GetUniqueID());
                if (entity) {
                    let tempdis = hitActor.K2_GetActorLocation().op_Subtraction(localCation).Size();
                    if (tempdis < nearestDis) {
                        nearestActor = hitActor;
                        nearestDis = tempdis;
                    }
                }
            }
        }
        return nearestActor;
    }
    /**
     * 看下lookActor
     * @param lookActor 朝向目标
     */
    LookAtActor(lookActor) {
        let aCharacter = this.GetUEActor();
        if (!aCharacter) {
            Logger_1.Logger.error("[BaseCharacter:LookAtActor] not Character");
            return;
        }
        let lookRotation = UE.KismetMathLibrary.FindLookAtRotation(aCharacter.K2_GetActorLocation(), lookActor.K2_GetActorLocation());
        lookRotation.Pitch = 0;
        aCharacter.K2_SetActorRotation(lookRotation, false);
        Logger_1.Logger.log("hitRsult 面向敌人 " + lookActor.GetName());
    }
    /** upate  */
    UpdateHandle;
    /**
     * 设置Update开启关闭
     * @param b 是否开启Update
     * @returns
     */
    SetUpdateEnable(b) {
        if (b) {
            if (this.UpdateHandle) {
                return;
            }
            //let dt = 1 / 60 * 1000;//秒
            //this.UpdateHandle = setInterval(this.Update.bind(this, dt / 1000), dt);
            ActivityMgr_1.default.GetInstance().AddGameTickListener(this.GameTick, this);
        }
        else {
            if (this.UpdateHandle) {
                clearInterval(this.UpdateHandle);
                this.UpdateHandle = null;
            }
            ActivityMgr_1.default.GetInstance().RemoveGameTickListener(this.GameTick, this);
        }
    }
    EngineTickDisTime = 0;
    UpdateDis = 1 / 60;
    /**引擎tick */
    GameTick(dt) {
        /**蓝图更新不限制 影响动作 */
        /**限制更新帧率 */
        this.EngineTickDisTime += dt;
        if (this.EngineTickDisTime >= this.UpdateDis) {
            this.Update(this.EngineTickDisTime);
            this.EngineTickDisTime = 0;
        }
    }
    /**设置是否可以动画跟运动 */
    SetRootMotionEnable(b) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            let ani = aCharacter.Mesh.GetAnimInstance();
            if (!ani) {
                return;
            }
            if (b) {
                ani.SetRootMotionMode(this.RootMotionDefault);
            }
            else {
                ani.SetRootMotionMode(UE.ERootMotionMode.IgnoreRootMotion);
            }
        }
    }
    //update
    Time_UpdateHalfSecond = 0.1;
    Timd_updateOneSecond = 0.01;
    /**
     * 调用最小间隔时间1/60s
     */
    Update(dt) {
        //一秒更新一次
        if (this.Timd_updateOneSecond > 0) {
            this.Timd_updateOneSecond -= dt;
            if (this.Timd_updateOneSecond <= 0) {
                this.Timd_updateOneSecond = 1;
                this.UpdateOneSecond(this.Timd_updateOneSecond);
            }
        }
    }
    /**每秒更新一次 */
    UpdateOneSecond(dt) {
    }
    /**隐藏在游戏中 */
    SetHiddenInGame(b) {
        if (this.HideInGame == b) {
            return false;
        }
        this.HideInGame = b;
        this.GetUEActor().SetActorHiddenInGame(b);
        return true;
    }
    /**
     * 播放一个粒子效果
     * @param particle
     * @param pos
     * @param rot
     * @returns
     */
    PlayParticle(particle, pos, rot) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            Logger_1.Logger.error("[BaseCharacter:PlayParticle] not Character");
            return null;
        }
        if (!particle || particle == "") {
            return null;
        }
        let p = AssetMgr_1.default.GetInstance().LoadParticle(particle);
        if (!p) {
            return null;
        }
        if (!pos) {
            let capsuleHight = aCharacter.CapsuleComponent.GetScaledCapsuleHalfHeight();
            pos = new UE.Vector(0, 0, capsuleHight / 2 * 3); //相对于网格体的位置 不是世界位置
        }
        if (!rot) {
            rot = new UE.Rotator(0, 90, 0); //相对旋转 不是世界旋转
        }
        let pp = UE.GameplayStatics.SpawnEmitterAttached(p, aCharacter.Mesh, undefined, pos, rot, undefined, UE.EAttachLocation.SnapToTarget, true);
        return pp;
    }
    /**
     * 添加一个actor到自己的actor上 AttachToActor
     * @param actor
     * @param slot
     * @param newScale
     */
    AddActorToSelf(addActor, slot, newScale) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            FunTool_1.default.AddActorToParent(aCharacter, addActor, slot, newScale);
            //胶囊体设置
            if (aCharacter.CapsuleComponent) {
                let hightValue = aCharacter.CapsuleComponent.GetUnscaledCapsuleHalfHeight();
                addActor.K2_AddActorLocalOffset(new UE.Vector(0, 0, -hightValue), false, undefined, false);
                Logger_1.Logger.log("[BaseCharacter:AddActorToSelf]设置位置偏移：hight ", -hightValue);
            }
        }
    }
    /**
     *
     * @returns 角色所在位置
     */
    GetLocation() {
        return this.UECharacter.K2_GetActorLocation();
    }
    /**移动到Actor位置 */
    MoveToActor(actor) {
        let aCharacter = this.GetUEActor();
        if (aCharacter) {
            let controll = aCharacter.GetController();
            UE.AIBlueprintHelperLibrary.SimpleMoveToActor(controll, actor);
        }
    }
    /**移动到 */
    MoveTo(pos) {
        let controll = this.UECharacter.GetController();
        if (!controll) {
            Logger_1.Logger.error("ERROR 不能进行移动，没有控制器");
        }
        else {
            UE.AIBlueprintHelperLibrary.SimpleMoveToLocation(controll, pos);
        }
    }
    /**
     * 设置位置
     */
    SetLocation(pos) {
        this.UECharacter.K2_SetActorLocation(pos, false, null, false);
    }
    /**
     * 设置网格体
     * @param newMesh
     * @param bReinitPose
     */
    SetSketonMesh(newMesh, bReinitPose) {
        this.UECharacter.Mesh.SetSkeletalMesh(newMesh, bReinitPose);
    }
    /**播放蒙太奇动画 */
    PlayMontage(montage, StartSectionName /* = "None" */, InPlayRate /* = 1.000000 */) {
        if (!this.UECharacter) {
            Logger_1.Logger.error(" this.ArrivalCharacter is null");
            return;
        }
        let aniMontage = AssetMgr_1.default.GetInstance().LoadMontage(montage);
        Logger_1.Logger.log("播放蒙太奇动画 " + montage);
        this.UECharacter.PlayAnimMontage(aniMontage, 1.0, StartSectionName);
    }
    /**
     * 设置当前motage的播放速度
     * @param newRate 播放速度
     */
    SetCurMontagePlayRate(newRate) {
        let curMontage = this.UECharacter.GetCurrentMontage();
        if (curMontage && this.BaseAniminstance) {
            this.BaseAniminstance.AnimInstance.Montage_SetPlayRate(curMontage, newRate);
        }
    }
    /**
     * 设置动画播放速度
     * @param rate
     */
    SetAnimPlayRate(rate) {
        this.UECharacter.Mesh.SetPlayRate(rate);
    }
    /**
     * 停止动画播放
     */
    StopAnimation() {
        this.UECharacter.Mesh.Stop();
    }
    /**
     * 播放动画
     */
    PlayAnimation(anim, loop) {
        let asset = AssetMgr_1.default.GetInstance().LoadAnimationAssest(anim);
        this.UECharacter.Mesh.PlayAnimation(asset, loop);
    }
    /**
     * 停止蒙太奇
     * @param AnimMontage
     */
    StopAnimMontage(AnimMontage /* = None */) {
        this.UECharacter.StopAnimMontage(AnimMontage);
        Logger_1.Logger.log("停止蒙太奇动画");
    }
    /**
     *  收到动画事件，只是skelton notify事件
     * @param eventName 事件名称
     * @param eventParam 事件参数
     * @returns 处理了此事件返回true 否则返回false
     */
    OnAnimNotify(eventName, eventParam) {
        Logger_1.Logger.todo("BaseCharacter OnAnimNotify");
        return false;
    }
    /**收到碰撞信息 */
    OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit) {
        Logger_1.Logger.log("OnActorHit Component" + Hit.Component.GetName());
        Logger_1.Logger.log("OnActorHit OtherActor" + OtherActor.GetName());
        Logger_1.Logger.log("OnActorHit OtherComp" + OtherComp.GetName());
    }
    /**移动更新句柄 */
    SplineMoveUpdateHanle;
    /**移动步长 */
    SplineMoveStep = 3;
    /**已经移动的长度 */
    SplineIsMovedLength = 0;
    /**线条总长度 */
    SplineTotaLength = 0;
    /**组件 */
    SplineCom;
    /**Spline组件的Actor */
    SplineActor;
    GetAIController() {
        let character = this.GetUEActor();
        let aiCon = character.GetController();
        return aiCon;
    }
    /**
     * 设置Spline组件
     */
    SetSplineComponet(splineCom) {
        this.SplineCom = splineCom;
    }
    /**更新多点移动路径 */
    UpdateSplineMovePath(points, pointType = UE.ESplinePointType.CurveClamped) {
        if (!this.SplineCom) {
            if (!this.SplineActor) {
                Logger_1.Logger.log("[BaseCharacter:UpdateSplineMovePath] Init SplineActor ");
                let splineActor = AssetMgr_1.default.GetInstance().LoadBP("Blueprint'/Game/BluePrints/BP_SplineActor.BP_SplineActor_C'");
                let actor = this.GetUEActor();
                let spactor = actor.GetWorld().SpawnActorByClass(splineActor, actor.K2_GetActorLocation(), actor.K2_GetActorRotation(), UE.ESpawnActorCollisionHandlingMethod.AlwaysSpawn);
                this.SplineActor = spactor;
            }
            this.SplineCom = this.SplineActor.Spline;
        }
        if (this.SplineCom) {
            Logger_1.Logger.log("[BaseCharacter:UpdateSplineMovePath] do SplineCom ");
            this.SplineCom.SetSplineWorldPoints(points);
            let pointNum = points.Num();
            for (let i = 0; i < pointNum; ++i) {
                this.SplineCom.SetSplinePointType(i, pointType);
            }
        }
    }
    /**
     * 传入路径点进行移动
     * @param points 路径点
    * @param moveStep 移动步 默认3
    * @param interval 移动间隔时间 默认1/30  秒
     */
    MoveWithKeyPointsBySpline(points, moveStep = 3, interval = 1 / 30) {
        let tempArr = GameMgr_1.default.GetInstance().GetTSEngine().TempVectorArray;
        tempArr.Empty();
        points.forEach(element => {
            Logger_1.Logger.log("[BaseCharacter:MoveWithKeyPointsBySpline] X ", element.X, " Y ", element.Y, " Z ", element.Z);
            tempArr.Add(element);
        });
        this.UpdateSplineMovePath(tempArr);
        if (this.SplineCom) {
            Logger_1.Logger.log("[BaseCharacter:MoveWithKeyPointsBySpline] do here ");
            this.SplineIsMovedLength = 0;
            this.SplineMoveStep = moveStep;
            this.StopMoveInSpline();
            this.SplineTotaLength = this.SplineCom.GetSplineLength();
            this.SplineMoveUpdateHanle = setInterval(this.UpdatePositionInSpline.bind(this), 1000 * interval);
        }
        else {
            Logger_1.Logger.error("MoveWithPoints not find this.SplineCom can not move ");
        }
    }
    /**
     * 移动结束
     */
    OnMoveWithKeyPointsBySplineFinish() {
    }
    /**Spine移动中的transform */
    SpineTransform;
    /**
     * 更新角色的Spline位置
     */
    UpdatePositionInSpline() {
        if (this.SplineCom) {
            let splineTrans = this.SplineCom.GetTransformAtDistanceAlongSpline(this.SplineIsMovedLength, UE.ESplineCoordinateSpace.World);
            this.SpineTransform = splineTrans;
            this.OnSplineMove(splineTrans);
            this.SplineIsMovedLength += this.SplineMoveStep;
            // Logger.log("[BaseCharacter:UpdatePositionInSpline] SplineIsMovedLength ",this.SplineIsMovedLength)
            let splineLenth = this.SplineTotaLength;
            if (this.SplineIsMovedLength >= splineLenth) {
                Logger_1.Logger.log("[BaseCharacter:UpdatePositionInSpline] Move finish ");
                this.SplineIsMovedLength = splineLenth;
                this.StopMoveInSpline();
                this.OnMoveWithKeyPointsBySplineFinish();
            }
        }
    }
    /**收到Spline的移动信息 */
    OnSplineMove(trans) {
        let moveActor = this.UECharacter;
        if (moveActor) {
            let pos = trans.GetLocation();
            Logger_1.Logger.log("[BaseCharacter:OnSplineMove] pos ", pos.X, " Y ", pos.Y);
            trans.SetLocation(new UE.Vector(pos.X, pos.Y, moveActor.K2_GetActorLocation().Z));
            moveActor.K2_SetActorTransform(trans, false, null, false);
        }
    }
    /**停止移动 */
    StopMoveInSpline() {
        if (this.SplineMoveUpdateHanle) {
            Logger_1.Logger.warn("[BaseCharacter:StopMoveInSpline]沿线移动结束 this.SplineIsMovedLength=" + this.SplineIsMovedLength, " this.SplineTotaLength=" + this.SplineTotaLength);
            clearInterval(this.SplineMoveUpdateHanle);
            this.SplineMoveUpdateHanle = null;
        }
    }
    /**
     * 获取导航路径点
     * @param goalPos
     * @returns
     */
    GetNaviGationPaths(goalPos) {
        Logger_1.Logger.warn("AIMoveToLocaltion");
        let startPos = this.GetUEActor().K2_GetActorLocation();
        let navigationPath = UE.NavigationSystemV1.FindPathToLocationSynchronously(GameMgr_1.default.GetInstance().GetWorld(), startPos, goalPos, this.GetUEActor());
        let paths = navigationPath.PathPoints;
        let count = paths.Num();
        if (count > 0) {
            for (let i = 0; i < count; i++) {
                Logger_1.Logger.warn("navigationPath " + i, "px = " + paths.Get(i).X, "py = " + paths.Get(i).Y, "pz = " + paths.Get(i).Z);
            }
        }
        else {
            Logger_1.Logger.warn("AIMoveToLocaltion 寻路点为0 ");
        }
        return paths;
    }
    /**给一个目标点，使用spline方式得到路径移动 */
    MoveToLocaltionInSpline(goalPos) {
        let paths = this.GetNaviGationPaths(goalPos);
        let count = paths.Num();
        if (count > 0) {
            for (let i = 0; i < count; i++) {
                Logger_1.Logger.warn("navigationPath " + i, "px = " + paths.Get(i).X, "py = " + paths.Get(i).Y, "pz = " + paths.Get(i).Z);
            }
            let arrPaths = FunTool_1.default.UEArray2TSArray(paths);
            this.MoveWithKeyPointsBySpline(arrPaths);
        }
        else {
            Logger_1.Logger.warn("AIMoveToLocaltion 寻路点为0 ");
        }
    }
    /**
     * 清理并停止KeyPoints移动
     */
    CleanKeyPointsSplineMove() {
        this.StopMoveInSpline();
        if (this.SplineActor) {
            this.SplineActor.K2_DestroyActor();
            this.SplineActor = null;
        }
        this.SetSplineComponet(null);
    }
    MoveToLocationWithAI(targetPos, theta) {
        let moveprox = UE.AIBlueprintHelperLibrary.CreateMoveToProxyObject(this.GetUEActor(), this.GetUEActor(), targetPos, undefined, 5.0, false);
        if (moveprox) {
            moveprox.OnSuccess.Add(this.OnMoveToLocationByAISuccess.bind(this));
            moveprox.OnFail.Add(this.OnMoveToLocationByAIFail.bind(this));
        }
        else {
            Logger_1.Logger.warn("[BaseCharacter:MoveToLocationWithAI] not MoveProxy");
        }
        //UE.AIBlueprintHelperLibrary.SimpleMoveToLocation(this.AIController, targetPos)
        //let moveprox = this.AIController.MoveToLocation(targetPos, 5.0, false, true, true, false);
        //UE.NavigationSystemV1.SimpleMoveToLocation(this.AIController, targetPos)
        //this.OnNaviToGoal(null, true);
    }
    SimpleMoveToLocaltion(targetPos) {
        let control = this.UECharacter.GetController();
        UE.AIBlueprintHelperLibrary.SimpleMoveToLocation(control, targetPos);
    }
    //keyPoints 移动通过AI方式//////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**AI移动keypoints */
    MoveByAiControlKeyPoints;
    theta;
    /**多点路径移动下一关键点 */
    KeyPointIndex = -1;
    MoveWithKeyPointsByAI(points) {
        this.UpdateSplineMovePath(FunTool_1.default.Vectors2UEArray(points));
        this.MoveByAiControlKeyPoints = points;
        this.KeyPointIndex = 0;
        this.MoveWithKeyPointByAINext();
    }
    /**
     * 移动到下一个关键点
     * @returns
     */
    MoveWithKeyPointByAINext() {
        this.KeyPointIndex += 1;
        let curIndex = this.KeyPointIndex;
        if (curIndex < 0) {
            Logger_1.Logger.error("MoveWithKeyPointNext KeyPointIndex error,  can not move");
            return;
        }
        if (curIndex >= this.MoveByAiControlKeyPoints.length) {
            return;
        }
        let pos = this.MoveByAiControlKeyPoints[curIndex];
        this.OnMoveKeyPointsByAINext(pos, this.theta);
    }
    /**
     * AI移动到下一个关键点
     */
    OnMoveKeyPointsByAINext(pos, theta) {
        this.MoveToLocationWithAI(pos, theta);
    }
    /**
     * AI移动成功
     * @param movementResult
     */
    OnMoveToLocationByAISuccess(movementResult) {
        Logger_1.Logger.warn("BaseCharacter OnMoveToLocationComplete bSuccess:" + "true", "movementResult:" + movementResult.toString());
        this.OnNaviToGoalComplete(true, movementResult);
    }
    /**
     * AI移动失败
     * @param movementResult
     */
    OnMoveToLocationByAIFail(movementResult) {
        Logger_1.Logger.warn("BaseCharacter OnMoveToLocationComplete bSuccess:" + "false", "movementResult:" + movementResult.toString());
        this.OnNaviToGoalComplete(false, movementResult);
    }
    /**
    * 停止关键点的移动
    */
    StopAIMove() {
        // AI移动停止 不知啥API 暂时用这种方式停止
        let pos = this.GetUEActor().K2_GetActorLocation();
        //this.MoveToLocationWithAI(pos);
        Logger_1.Logger.log(" BaseCharacter StopKeyPointsMove ");
        this.KeyPointIndex = -1;
    }
    /**
     * 导航到目标点完成
     * @param bSuccess
     * @param errorCode
     */
    OnNaviToGoalComplete(bSuccess, errorCode) {
        Logger_1.Logger.log(" BaseCharacter OnNaviToGoalComplete " + bSuccess, " errCode:" + errorCode, " this.KeyPointIndex:" + this.KeyPointIndex);
        if (this.KeyPointIndex >= 0) {
            if (this.KeyPointIndex >= this.MoveByAiControlKeyPoints.length) {
                this.StopAIMove();
                this.OnMoveWithKeyPointsByAIFinish();
            }
            else {
                this.MoveWithKeyPointByAINext();
            }
        }
    }
    /**
     * 关键点移动完成
     */
    OnMoveWithKeyPointsByAIFinish() {
    }
}
exports.default = BaseCharacter;
//# sourceMappingURL=BaseCharacter.js.map