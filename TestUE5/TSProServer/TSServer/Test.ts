import { Logger } from "Framework/Common/Logger";
import * as UE from "ue";

export default class Test
{

    callback: (aa: string, bb: number) => {};
    Run()
    {
        this.callback("234", 1);
    }

    static Run2()
    {

        let testaa: A11 = new A22();


        let a33 = testaa as A33;
        if (a33)
        {
            // a33.testA33();
        }

        if (testaa instanceof A33)
        {
            testaa.testA33();
        }

        let obj = { a: 1, b: 2 };
        let arr = new Array<number>();

    }

}


class testB
{
    start()
    {
        let a = new Test();
        a.callback = this.testFun.bind(this);
    }

    testFun(aa: string, bb: number)
    {
        Logger.log(aa, bb);
    }
}

let testb = new testB();
testb.start();

class DataTable
{
    public columns: Array<string> = new Array<string>();
    public rows: Array<DataRow> = new Array<DataRow>();
}
class DataRow
{
    public cells: Array<string> = new Array<string>();
}



//const jsonString =  '{"employee":{ "name":"John", "age":30, "city":"New York" }}';

const jsonString = '{"question":{"text": "赶紧的","lang": "CH","questionId": "864972045011561_20220513205918873"},"answer": [{"source": "common_sense_qa","asr":{"text": "赶紧的","lang": "CH","questionId": "864972045011561_20220513205918873"},"push": [],"operation":{"name": "$QA","params":{}},"iscredible": false,"confidence": 0.73407310000000003,"tts": [{"text": "刚几点啊","lang": "CH","action":{"name": "","param":{"domain": "","duration": 0,"extra_params":{},"frame_no": 0,"guide_tip": "","intent": "","params":{},"pic_url": "","play_type": "","url": "","video_url": ""},"display": ""},"audio": "","emoji": "","payload": "{}","type": ""}],"tags": [],"recomendation": [],"similars": ["赶紧起"],"tree":{"currentstate": "","subtree": []},"expiration": 36000,"emotion":{"name": "none","score": 0.98925822973251343},"score": 1,"responder": "HI"}],"responder": "CLOUD"}';



//自定义类型
export type MyData =
    {
        Damage: number[];
        Proportion?: number[];
        Power: number[];
        CDTime: number;
        SkillType: number;
        SkillShowName: string;

    }


interface testInterface
{
    GetAttribute?(): void;
    Print();
}

class A11 implements testInterface
{
    Member1: number;
    Member2: Array<string>;
    Member3: boolean;
    Member4: Function;
    Member5: MyData;

    Print()
    {
        Logger.log("testInterface ddddddddddddddddddddddd");
    }
    TestFun()
    {
        Logger.log("this.is A TestFun");
    }
}






class A22 extends A11
{
    TestFun(): void
    {
        Logger.log("this.is A22 TestFun");
    }

}

class A33 extends A22
{
    TestFun(): void
    {
        Logger.log("this.is A33 TestFun");
    }

    testA33()
    {

    }
}

var ue5 = function (r)
{
    Logger.log('ue5 func:' + r);
    return true ? (function (t, e, n, o)
    {
        Logger.log(2, 3, 4, 5);
    }
    ) :
        (
            function (t, e, n, o)
            {
                Logger.log(t, e, n, o);
            });
    var i
    Logger.log("return ue5 func obj");
}(function (t, e)
{
    Logger.log("function ", t, e)
});

//ue5(1, 2, 3, 4);

