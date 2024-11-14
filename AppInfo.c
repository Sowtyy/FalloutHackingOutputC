#pragma once
#ifndef APPINFO
#define APPINFO

struct AppInfo
{
    const char* name;
    const char* version;
    const char* versionDate;
    const char* author;
};

static struct AppInfo get_app_info()
{
    struct AppInfo appInfo = { 0 };
    appInfo.name = "FalloutHackingOutputC";
    appInfo.version = "1.0.0";
    appInfo.versionDate = "2024/11/14";
    appInfo.author = "Sowtyy";
    return appInfo;
}

#endif
