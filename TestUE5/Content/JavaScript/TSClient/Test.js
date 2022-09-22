"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Test {
    callback;
    Run() {
        this.callback("234", 1);
    }
    static Run2() {
        let testaa = new A22();
        let a33 = testaa;
        if (a33) {
            // a33.testA33();
        }
        if (testaa instanceof A33) {
            testaa.testA33();
        }
        let obj = { a: 1, b: 2 };
        let arr = new Array();
        /*
        let AA = eval('class AA { say() { console.log("jkjkjkjkj123"); } } exports.AA = AA')
        let xx = new AA();
        xx.say();
        */
    }
}
exports.default = Test;
class testB {
    start() {
        let a = new Test();
        a.callback = this.testFun.bind(this);
    }
    testFun(aa, bb) {
        console.log(aa, bb);
    }
}
let testb = new testB();
testb.start();
class DataTable {
    columns = new Array();
    rows = new Array();
}
class DataRow {
    cells = new Array();
}
//const jsonString =  '{"employee":{ "name":"John", "age":30, "city":"New York" }}';
const jsonString = '{"question":{"text": "赶紧的","lang": "CH","questionId": "864972045011561_20220513205918873"},"answer": [{"source": "common_sense_qa","asr":{"text": "赶紧的","lang": "CH","questionId": "864972045011561_20220513205918873"},"push": [],"operation":{"name": "$QA","params":{}},"iscredible": false,"confidence": 0.73407310000000003,"tts": [{"text": "刚几点啊","lang": "CH","action":{"name": "","param":{"domain": "","duration": 0,"extra_params":{},"frame_no": 0,"guide_tip": "","intent": "","params":{},"pic_url": "","play_type": "","url": "","video_url": ""},"display": ""},"audio": "","emoji": "","payload": "{}","type": ""}],"tags": [],"recomendation": [],"similars": ["赶紧起"],"tree":{"currentstate": "","subtree": []},"expiration": 36000,"emotion":{"name": "none","score": 0.98925822973251343},"score": 1,"responder": "HI"}],"responder": "CLOUD"}';
class A11 {
    Member1;
    Member2;
    Member3;
    Member4;
    Member5;
    Print() {
        console.log("testInterface ddddddddddddddddddddddd");
    }
    TestFun() {
        console.log("this.is A TestFun");
    }
}
class A22 extends A11 {
    TestFun() {
        console.log("this.is A22 TestFun");
    }
}
class A33 extends A22 {
    TestFun() {
        console.log("this.is A33 TestFun");
    }
    testA33() {
    }
}
var ue5 = function (r) {
    console.log('ue5 func:' + r);
    return true ? (function (t, e, n, o) {
        console.log(2, 3, 4, 5);
    }) :
        (function (t, e, n, o) {
            console.log(t, e, n, o);
        });
    var i;
    console.log("return ue5 func obj");
}(function (t, e) {
    console.log("function ", t, e);
});
//ue5(1, 2, 3, 4);
//# sourceMappingURL=Test.js.map