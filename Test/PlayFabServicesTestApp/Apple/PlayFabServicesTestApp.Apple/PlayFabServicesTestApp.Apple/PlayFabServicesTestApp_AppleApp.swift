//
//  PlayFabServicesTestApp_AppleApp.swift
//  PlayFabServicesTestApp.Apple
//

import SwiftUI

@main
struct PlayFabServicesTestApp_AppleApp: App {
    //@UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate

#if os(iOS)
    @UIApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
#elseif os(macOS)
    @NSApplicationDelegateAdaptor(AppDelegate.self) var appDelegate
#endif
    
    var body: some Scene {
        WindowGroup {
            ContentView()
        }
    }
}
