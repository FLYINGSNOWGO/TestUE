"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.default = void 0;
const DataType_1 = require("Framework/Config/DataType");
const EntityMgr_1 = require("Framework/Manager/EntityMgr");
/**
 * 所有Actor实体基类，
 */
class BaseActor {
    /**UE的C++对象 以最终父类的类型为准*/
    UEActor;
    EntityTags = [DataType_1.EEntityTag.None];
    /**重置对象所有数据**/
    ResetData() {
        this.UEActor = null;
    }
    //多重继承的基类不能有静态函数
    static Create(C, character) {
        let Ret = new C();
        Ret.Init(character);
        return Ret;
    }
    /**初始化 */
    Init(aUIActor) {
        this.ResetData();
        this.UEActor = aUIActor;
        EntityMgr_1.default.RegeisterEntity(this);
    }
    /**需要在基类中调用 */
    Clean() {
        EntityMgr_1.default.UnRegeisterEntity(this);
        this.ResetData();
    }
    GetActorAS() {
        let actor = this.GetUEActor();
        return actor;
    }
    GetUEActor() {
        return this.UEActor;
    }
    SetUEActor(aUEActor) {
        this.UEActor = aUEActor;
    }
    GetUEActorName() {
        return this.UEActor.GetName();
    }
    GetUObjectUniqueID() {
        return this.UEActor.GetUniqueID();
    }
    GetEntityTages() {
        return this.EntityTags;
    }
    GetEntityName() {
        return this.constructor.name;
    }
    /**
     * 销毁Actor
     * @param delay 大于0则延迟销毁，不传则立即销毁
     */
    DestroyActor(delay) {
        if (this.GetUEActor()) {
            if (delay) {
                setTimeout(() => {
                    this.GetUEActor().K2_DestroyActor();
                    this.Clean();
                }, delay * 1000);
            }
            else {
                this.GetUEActor().K2_DestroyActor();
                this.Clean();
            }
        }
    }
}
exports.default = BaseActor;
//# sourceMappingURL=BaseActor.js.map