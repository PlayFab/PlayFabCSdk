#pragma once

namespace PlayFab
{
// DO NOT EDIT sdkVersion BY HAND. It is set automatically by the official build pipelines
// (Pipelines/Scripts/Set-SdkVersion.ps1 via Pipelines/Tasks/set-sdk-version.yml) before compiling:
//   format   YYMM.QFE.R.YYMMDD  (GDK release . QFE number . standalone release revision . build date)
//   YYMM/QFE ADO pipeline variables PFSdkRelease / PFSdkQfe, or the YYMM.QFE.R release tag
//   R        the R of the YYMM.QFE.R release tag being built, otherwise 0
//   YYMMDD   the pipeline run's start date
// "0000.0.0.000000" is the placeholder for local / from-source builds that were not stamped by CI.
constexpr char sdkVersion[] = "2610.0.2.261029";
constexpr char versionString[] = "PFCSdk-";
constexpr char userAgent[] = "PFCSdk/";

constexpr char win32Plat[] = "WIN32";
constexpr char gdkPlat[] = "GDK";
constexpr char nintendoPlat[] = "NINTENDO";
constexpr char ps4Plat[] = "SONY_PLAYSTATION_4";
constexpr char ps5Plat[] = "SONY_PLAYSTATION_5";
constexpr char linuxPlat[] = "LINUX";
constexpr char iosPlat[] = "IOS";
constexpr char macosPlat[] = "MACOS";
constexpr char androidPlat[] = "ANDROID";

constexpr char buildOrigin[] = "SOURCE";
}
