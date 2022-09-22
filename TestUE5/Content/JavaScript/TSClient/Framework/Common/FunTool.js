"use strict";
/*
*  静态工具函数类
*/
Object.defineProperty(exports, "__esModule", { value: true });
const UE = require("ue");
const puerts_1 = require("puerts");
const AssetMgr_1 = require("Framework/Manager/AssetMgr");
const GameMgr_1 = require("Framework/Manager/GameMgr");
const Logger_1 = require("./Logger");
class FunTools {
    /**
     * 生成范围随机数
     * @Min 最小值
     * @Max 最大值
     */
    static GetRandomNum(Min, Max) {
        var Range = Max - Min;
        var Rand = Math.random();
        return (Min + Math.round(Rand * Range));
    }
    static GetRecentlyActor(actor, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType) {
        let hitRsult = UE.TSHelper.GetRecentlyActorByTrace(actor, Start, End, Radius, TraceChannel, bTraceComplex, ActorsToIgnore, DrawDebugType, UE.ECheckTraceType.ObjectType);
        if (hitRsult.Num() > 0) {
            let hitActor = hitRsult.Get(0).HitObjectHandle.Actor;
            let lookRotation = UE.KismetMathLibrary.FindLookAtRotation(actor.K2_GetActorLocation(), hitActor.K2_GetActorLocation());
            return lookRotation;
        }
        else {
            return null;
        }
    }
    // 获取升序number数组中第一个大于或者等于给定数值的索引和数值 如果没有则返回索引 idx:-1, val:0
    static AscArrayFirstGreatOrEqualNumber(arr, value) {
        var len = arr.length;
        if (len == 0) {
            return { idx: -1, val: 0 };
        }
        if (value < arr[0]) {
            return { idx: -1, val: 0 };
        }
        if (arr[len - 1] < value) {
            return { idx: -1, val: 0 };
        }
        var index = 0;
        for (; len > 0;) {
            let step = len >> 1;
            let next = index + step;
            if (arr[next] < value) {
                index = next + 1;
                len -= step + 1;
            }
            else {
                len = step;
            }
        }
        return { idx: index, val: arr[index] };
    }
    // 获取升序number数组中最后一个小于或者等于给定数值的索引和数值 如果没有则返回索引 idx:-1, val:0
    static ASCArrayLastLessOrEqualNumber(arr, value) {
        var len = arr.length;
        if (len == 0) {
            return { idx: -1, val: 0 };
        }
        if (value < arr[0]) {
            return { idx: -1, val: 0 };
        }
        let index = len - 1;
        for (; len > 0;) {
            let step = len >> 1;
            let next = index - step;
            if (arr[next] > value) {
                index = next - 1;
                len -= step + 1;
            }
            else {
                len = step;
            }
        }
        return { idx: index, val: arr[index] };
    }
    static PlayParticle(particle, pos, scale, rot) {
        if (!particle || particle == "") {
            return null;
        }
        let p = AssetMgr_1.default.GetInstance().LoadParticle(particle);
        if (!p) {
            return null;
        }
        let ss = undefined;
        if (scale) {
            ss = scale;
        }
        let rr = undefined;
        if (rot) {
            rr = rot;
        }
        let pp = UE.GameplayStatics.SpawnEmitterAtLocation(GameMgr_1.default.GetInstance().GetWorld(), p, pos, rr, ss, true, UE.EPSCPoolMethod.AutoRelease, true);
        if (!pp) {
            return null;
        }
        return pp;
    }
    /**播放niagarag */
    static PlayNiagaraParticle(particle, pos, scale, rot) {
        let p = AssetMgr_1.default.GetInstance().LoadNiagaraParticle(particle);
        if (!p) {
            return null;
        }
        let ss = undefined;
        if (scale) {
            ss = scale;
        }
        let rr = undefined;
        if (rot) {
            rr = rot;
        }
        return UE.NiagaraFunctionLibrary.SpawnSystemAtLocation(GameMgr_1.default.GetInstance().GetWorld(), p, pos, rr, ss, true);
    }
    /**
     * 将{x:number,y:number,z:number} 转为UE.vector,x y z 必须有值，否则崩溃
     * @param param0
     */
    static XYZ2UEVector(xyz) {
        if (!xyz) {
            Logger_1.Logger.error("XYZ2UEVector:value error  xyz is null!!");
            return null;
        }
        return new UE.Vector(xyz.x, xyz.y, xyz.z);
    }
    /**
     * 将UE的Array转成TS中的Array
     * @param uearray UE的Array数据
     * @returns
     */
    static UEArray2TSArray(uearray) {
        let count = uearray.Num();
        let tsArray = new Array();
        for (let i = 0; i < count; i++) {
            tsArray.push(uearray.Get(i));
        }
        return tsArray;
    }
    /**
     * 转换 TS Array<UE.Vector> 到 UE.TArray<UE.Vector>
     * @param arr
     * @returns
     */
    static Vectors2UEArray(arr) {
        let tempArr = GameMgr_1.default.GetInstance().GetTSEngine().TempVectorArray;
        tempArr.Empty();
        arr.forEach(element => {
            tempArr.Add(element);
        });
        return tempArr;
    }
    static PlaySound(sound, pos) {
        if (!sound || sound == "") {
            return false;
        }
        let p = AssetMgr_1.default.GetInstance().LoadSound(sound);
        if (!p) {
            return false;
        }
        UE.GameplayStatics.PlaySoundAtLocation(GameMgr_1.default.GetInstance().GetWorld(), p, pos, new UE.Rotator(0, 0, 0));
        return true;
    }
    /**获取actor*/
    static GetActorsByTag(tag) {
        let outActors = (0, puerts_1.$ref)(undefined);
        UE.GameplayStatics.GetAllActorsWithTag(GameMgr_1.default.GetInstance().GetWorld(), tag, outActors);
        return (0, puerts_1.$unref)(outActors);
    }
    /**是否有tag */
    static ActorHasTag(actor, tag) {
        if (actor) {
            return actor.ActorHasTag(tag);
        }
        return false;
    }
    /**获取两个Acotr的距离 */
    static CalcDistForActor(MainActor, OtherActor) {
        let pos = OtherActor.K2_GetActorLocation();
        let myPos = MainActor.K2_GetActorLocation();
        return pos.op_Subtraction(myPos).Size();
    }
    /**添加一个actor到另一个Actor */
    static AddActorToParent(ParentActor, addActor, slot = "", newScale) {
        if (ParentActor && addActor) {
            addActor.K2_AttachToActor(ParentActor, slot, UE.EAttachmentRule.SnapToTarget, UE.EAttachmentRule.SnapToTarget, UE.EAttachmentRule.SnapToTarget, false);
            if (newScale) {
                addActor.SetActorScale3D(newScale);
            }
        }
    }
    static CCUToUELocation_Matrix(CCU_Pos, nTheta) {
        return UE.TSHelper.CCUToUELocation_Matrix(CCU_Pos, nTheta, GameMgr_1.default.GetInstance().Scene_CUU_To_UE);
        // return GameMgr.GetInstance().Scene_CUU_To_UE.TransformPosition(CCU_Pos);
    }
    static UEToCCULocation_Matrix(UE_Pos) {
        // return GameMgr.GetInstance().Scene_UE_To_CCU.TransformPosition(UE_Pos);
        return UE.TSHelper.UEToCCULocation_Matrix(UE_Pos, GameMgr_1.default.GetInstance().Scene_UE_To_CCU);
    }
}
exports.default = FunTools;
//# sourceMappingURL=FunTool.js.map