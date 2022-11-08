// Fill out your copyright notice in the Description page of Project Settings.


#include "Tool/GameCheatManager.h"
#include "Serialization/ArrayReader.h"

DEFINE_LOG_CATEGORY(LogGameCheatManager)

union data2char {
	uint8 uc[4];
	uint32 ui;
	float ff;
};

void UGameCheatManager::Proc_JointStateControl(std::string databuf) 
{
	
	unsigned char* ucBufPtr =
		(unsigned char*)databuf.c_str() + 16;  // skip total header
	/*unsigned char* ucTmpPtr;
	unsigned char* ucScaDataPtr;
	unsigned char* ucTimePtr;*/
	//db2char dTmp;
	int group_cnt[6] = { 0 };

	//ucTmpPtr = (unsigned char*)databuf.c_str() + 20;
	//cm_bigEndianConvert(ucTmpPtr, dTmp.uc);
	//int js_size = (int)ucTmpPtr[8];
	//ROS_INFO("Proc_JointStateControl: data len %lu, frame len %d, js_size %d",
	//	databuf.size(), dTmp.ll, js_size);
	//if (js_size == (dTmp.ll / 5 - 1)) {
	//	ucScaDataPtr = ucTmpPtr + 9 + js_size;

	//	sensor_msgs::JointState js_msg;
	//	ginger_common_msgs::robot_rt_animation realtime_msg;
	//	ucTimePtr = (unsigned char*)databuf.c_str() + 24;
	//	cm_bigEndianConvert(ucTimePtr, dTmp.uc);
	//	ROS_INFO("Proc_JointStateControl: ,time %f", dTmp.dd);
	//	realtime_msg.time = dTmp.dd;

	//	for (int i = 0; i < js_size; i++) {
	//		int js_id = (int)ucTmpPtr[9 + i];
	//		// if (js_id > JOINTS_CNT_MAX + 4) {
	//		//   ROS_WARN("Proc_JointStateControl: js_id %d abnormal, skip ...",
	//		//   js_id); continue;
	//		// }

	//		js_msg.name.push_back(strjoints[js_id - 4]);
	//		cm_bigEndianConvert(ucScaDataPtr + 4 * i, dTmp.uc);
	//		js_msg.position.push_back(dTmp.dd);

	//		realtime_msg.id_list.push_back(js_id);
	//		realtime_msg.pos_list.push_back(dTmp.dd);
	//	}
	//	g_realtime_pub_.publish(realtime_msg);
	//}
	//else {
	//	ROS_ERROR("Proc_JointStateControl: js_size %d not match with data size %d",
	//		js_size, dTmp.ll);
	//}
}

FString UGameCheatManager::ArrayUint82String(const FArrayReader& ArrayReaderRef)
{

	int32 DataByteNum = ArrayReaderRef.Num();
	TArray<uint8> ReceivedData;
	for (int32 i = 0; i < DataByteNum;i++)
	{
		ReceivedData.Add(ArrayReaderRef[i]);
	}
	ReceivedData.Add('\0');
	//FString tryToString(reinterpret_cast<const char*>(ReceivedData.GetData()));
	FString tryToString = FString::Printf(TEXT("%s"), UTF8_TO_TCHAR(reinterpret_cast<const char*>(ReceivedData.GetData())));

	return tryToString;
}

void UGameCheatManager::TestArrayUint82String()
{
	FString FilePath = FPaths::ProjectSavedDir() / TEXT("PoseData.bin");
	TArray<uint8> Reader;
	if (!FFileHelper::LoadFileToArray(Reader,*FilePath))
	{
		UE_LOG(LogGameCheatManager,Error,TEXT("Failed to load file '%s'."),*FilePath);
		return;
	}
	FString Str;
	for (const uint8 Value : Reader)
	{
		Str.AppendInt(Value);
		Str.AppendChar(TEXT(' '));
	}

	std::string stdStr = TCHAR_TO_UTF8(*Str);

	const std::string dataStr(reinterpret_cast<const char*>(Reader.GetData()), Reader.Num());

	Proc_JointStateControl(dataStr);
	UE_LOG(LogGameCheatManager,Display,TEXT("TestArrayUint82String: ArrayReaderStr:%s."),*Str);
}

void UGameCheatManager::TestCreateBin()
{
	TArray<uint8> pData;
	pData.Add(1);
	pData.Add(3);
	pData.Add(13);
	FString str;
	for (uint8 i = 0;i < pData.Num();i++)
	{
		str.AppendInt(pData[i]);
	}
	UE_LOG(LogGameCheatManager, Display, TEXT("[TestCreateBin] Str:%s"),*str);
	FString OutputFile = FPaths::ProjectSavedDir() / TEXT("PoseData.bin");
	if (!FFileHelper::SaveArrayToFile(pData, *OutputFile, &IFileManager::Get(), FILEWRITE_Silent))
	{
		UE_LOG(LogGameCheatManager, Fatal, TEXT("Failed to create PoseData.bin file"));
	}
}

void UGameCheatManager::TestCodePoint()
{
	// FString Str = TEXT("你");
	// UE_LOG(LogGameCheatManager, Display, TEXT("[TestCodePoint] CodePointAt 你:%o"),(int)Str[0]);
}