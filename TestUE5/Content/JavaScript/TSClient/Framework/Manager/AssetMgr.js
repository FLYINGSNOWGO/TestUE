"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Logger_1 = require("Framework/Common/Logger");
const UE = require("ue");
const GameMgr_1 = require("./GameMgr");
/**
 * 资源管理器，所有资源请使用此类做加载，后面要做cache机制
 */
class AssetMgr {
    //单例模块
    constructor() { }
    ;
    static Instance;
    /**获取单例 */
    static GetInstance() {
        if (!this.Instance) {
            this.Instance = new AssetMgr();
        }
        return this.Instance;
    }
    /**初始化*/
    Init() {
    }
    LoadAssetAsync(filePath, callback) {
        let p = new Promise((resolve, reject) => {
            let asyncLoadObj = new UE.AsyncLoadState();
            asyncLoadObj.LoadedObjectCallback.Bind((obj) => {
                asyncLoadObj.LoadedObjectCallback.Unbind();
                if (obj) {
                    resolve(obj);
                }
                else {
                    reject(new Error(`load ${filePath} fail`));
                }
            });
            asyncLoadObj.StartLoadObject(filePath);
        });
        p.then(function (data) {
            console.log("载入资源完成 " + data.GetName());
            callback.call(this, data);
        }).catch(function (error) {
            Logger_1.Logger.log("载入资源失 败" + error);
            callback.call(this, null);
        });
    }
    /**载入一个UIWidget*/
    LoadWidget(UIWidgetBP) {
        let world = GameMgr_1.default.GetInstance().GetWorld();
        let uibp = UE.Class.Load(UIWidgetBP);
        let widget = UE.TSHelper.CreateWidget(world, uibp);
        return widget;
    }
    /**载入一个动画*/
    LoadAniSeq(AnimSeq) {
        let obj = this.CacheMap.get(AnimSeq);
        if (obj) {
            console.warn("缓存中取到 载入一个动画  ");
            return obj;
        }
        let ani = UE.AnimSequence.Load(AnimSeq);
        return ani;
    }
    /**载入一个蒙太奇动画*/
    LoadMontage(montageBP) {
        let obj = this.CacheMap.get(montageBP);
        if (obj) {
            console.warn("缓存中取到 载入一个蒙太奇动画  ");
            return obj;
        }
        let ani = UE.AnimMontage.Load(montageBP);
        return ani;
    }
    /**
     * 载入动画资源文件
     * @param assest
     * @returns 返回 UE.AnimationAsset 对象
     */
    LoadAnimationAssest(assest) {
        let obj = this.CacheMap.get(assest);
        if (obj) {
            return obj;
        }
        else {
            obj = UE.AnimationAsset.Load(assest);
            return obj;
        }
    }
    /**载入一个特效 */
    LoadParticle(particle) {
        let obj = this.CacheMap.get(particle);
        if (obj) {
            console.warn("缓存中取到 载入一个特效  ");
            return obj;
        }
        let ld = UE.ParticleSystem.Load(particle);
        if (!ld) {
            console.error("获取粒子数据失败：", particle);
            return null;
        }
        return ld;
    }
    /**载入Niagara粒子 */
    LoadNiagaraParticle(particle) {
        let obj = this.CacheMap.get(particle);
        if (obj) {
            console.warn("缓存中取到 载入Niagara粒子:" + particle);
            return obj;
        }
        let pa = UE.NiagaraSystem.Load(particle);
        if (pa) {
            return pa;
        }
        else {
            console.error("获取 Niagara 粒子数据失败：", particle);
            return null;
        }
    }
    /**载入一个蓝图类 */
    LoadBP(BP) {
        let obj = this.CacheMap.get(BP);
        if (obj) {
            console.warn("缓存中取到 载入一个蓝图类  ");
            return obj;
        }
        return UE.Class.Load(BP);
    }
    /**载入一个曲线资源*/
    LoadCurveBase(curve) {
        return UE.CurveBase.Load(curve);
    }
    /**创建一个角色 */
    CreateCharacter(bp, curPos, roa) {
        Logger_1.Logger.log("创建新角色" + bp);
        let BPClass = AssetMgr.GetInstance().LoadBP(bp);
        if (BPClass) {
            let ArrivalChar = GameMgr_1.default.GetInstance().GetWorld().SpawnActorByClass(BPClass, curPos, roa, UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn);
            //ArrivalChar.AutoPossessAI = UE.EAutoPossessAI.Spawned;
            return ArrivalChar;
        }
        return null;
    }
    /**创建AI PC */
    CreateAIPlayerController(bp, curPos, roa) {
        Logger_1.Logger.log("创建新AIPC" + bp);
        let BPClass = AssetMgr.GetInstance().LoadBP(bp);
        if (BPClass) {
            let ArrivalPC = GameMgr_1.default.GetInstance().GetWorld().SpawnActorByClass(BPClass, curPos, roa, UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn);
            //ArrivalChar.AutoPossessAI = UE.EAutoPossessAI.Spawned;
            return ArrivalPC;
        }
        return null;
    }
    /**创建AI PC */
    CreatePOI(bp, curPos, roa = new UE.Rotator()) {
        Logger_1.Logger.log("创建新POI:" + bp);
        let BPClass = AssetMgr.GetInstance().LoadBP(bp);
        if (BPClass) {
            let ArrivalPOI = GameMgr_1.default.GetInstance().GetWorld().SpawnActorByClass(BPClass, curPos, roa, UE.ESpawnActorCollisionHandlingMethod.AdjustIfPossibleButAlwaysSpawn);
            return ArrivalPOI;
        }
        return null;
    }
    /**获取锁定敌人Actor 标志 */
    LoadLockEnemyActor() {
        let BPClass = AssetMgr.GetInstance().LoadBP("Blueprint'/Game/Res/Role/Art/VFX/FX/ShootLockEnemyTag.ShootLockEnemyTag_C'");
        return BPClass;
    }
    /**载入一个音效 */
    LoadSound(sound) {
        let ld = UE.SoundBase.Load(sound);
        if (!ld) {
            Logger_1.Logger.error("加载音效失败", sound);
        }
        return ld;
    }
    /**载入2D纹理 */
    LoadTexture(path) {
        let ld = UE.Texture2D.Load(path);
        if (!ld) {
            Logger_1.Logger.error("加载Texture失败", path);
        }
        return ld;
    }
    /*载入一个材质实例 */
    LoadMaterialInstance(path) {
        let mi = UE.MaterialInstance.Load(path);
        if (!mi) {
            Logger_1.Logger.error("加载MaterialInstance失败", path);
        }
        return mi;
    }
    /**载入媒体视频资源 */
    LoadFileMediaSource(path) {
        return UE.FileMediaSource.Load(path);
    }
    //预加载特效
    PreLoadParticleArr = [
    //"ParticleSystem'/Game/Res/Role/Art/VFX/FX/Xiaoyan/F_UI_Fire3.F_UI_Fire3'",
    //"NiagaraSystem'/Game/Res/Role/Art/VFX/FX/Xiaoyan/daoguang.daoguang'",
    ];
    //预加载montage
    PreLoadMontageArr = [
    //"AnimMontage'/Game/Res/Role/Art/Actor/XiaoYan/BP_Ani/Countess_Login_P3ToP1.Countess_Login_P3ToP1'",
    ];
    //预加载音效
    PreLoadSound = [
    // "SoundWave'/Game/Res/Sound/Battle/WAV/Sord_role_04.Sord_role_04'",
    ];
    //蓝图预加载
    PreLoadBP = [
    // "Blueprint'/Game/Res/Weapon/Howitzer_Bullet_Attack_DelayedBomb.Howitzer_Bullet_Attack_DelayedBomb_C'",
    ];
    CacheMap = new Map();
    /**简单的预加载资源 暂用 */
    PreLoading() {
        console.log("资源预加载开始...", UE.KismetMathLibrary.UtcNow());
        let mgr = AssetMgr.GetInstance();
        this.PreLoadBP.forEach(element => {
            let obj = mgr.LoadBP(element);
            this.CacheMap.set(element, obj);
        });
        this.PreLoadMontageArr.forEach(element => {
            let obj = mgr.LoadMontage(element);
            this.CacheMap.set(element, obj);
        });
        this.PreLoadParticleArr.forEach(element => {
            if (element.startsWith("NiagaraSystem")) {
                let obj = mgr.LoadNiagaraParticle(element);
                this.CacheMap.set(element, obj);
            }
            else {
                let obj = mgr.LoadParticle(element);
                this.CacheMap.set(element, obj);
            }
        });
        this.PreLoadSound.forEach(element => {
            let obj = mgr.LoadSound(element);
            this.CacheMap.set(element, obj);
        });
        console.log("资源预加载结束...", UE.GameplayStatics.GetTimeSeconds(GameMgr_1.default.GetInstance().GetWorld()));
    }
}
exports.default = AssetMgr;
//# sourceMappingURL=AssetMgr.js.map