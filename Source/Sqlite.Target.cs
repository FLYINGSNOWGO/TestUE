// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SqliteTarget : TargetRules
{
	public SqliteTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
        // Logs and asserts are still useful to report results
        bUseLoggingInShipping = true;
        ExtraModuleNames.AddRange( new string[] { "Sqlite" } );
	}
}
