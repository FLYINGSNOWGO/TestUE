"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const Logger_1 = require("Framework/Common/Logger");
class EntityMgr {
    static EntityMap = new Map();
    static RegeisterEntity(entity) {
        let id = entity.GetUObjectUniqueID();
        if (!id) {
            Logger_1.Logger.error("Fail to register, Entity GetUObjectUniqueID is empty");
            return;
        }
        if (this.EntityMap.has(id)) {
            Logger_1.Logger.error("Fail to register, Entity GetUObjectUniqueID is exist");
        }
        Logger_1.Logger.warn("EntityMgr RegeisterEntity GetUObjectUniqueID:" + id + " UObjectName:" + entity.GetUEActorName());
        this.EntityMap.set(id, entity);
    }
    static GetEntity(uniqueid) {
        if (!uniqueid) {
            return null;
        }
        if (this.EntityMap.has(uniqueid)) {
            return this.EntityMap.get(uniqueid);
        }
        return null;
    }
    /** UnRegeisterEntity */
    static UnRegeisterEntity(entity) {
        let name = entity.GetUObjectUniqueID();
        if (this.EntityMap.has(name)) {
            return this.EntityMap.delete(name);
        }
        else {
            Logger_1.Logger.error("UnRegeisterEntity, Entity ${name} is not exist", name);
            return false;
        }
    }
}
exports.default = EntityMgr;
//# sourceMappingURL=EntityMgr.js.map