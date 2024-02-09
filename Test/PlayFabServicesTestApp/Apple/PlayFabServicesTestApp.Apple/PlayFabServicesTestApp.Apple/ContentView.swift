//
//  ContentView.swift
//  PlayFabServicesTestApp.Apple
//

import SwiftUI
import Darwin
import AuthenticationServices

struct ContentView: View {

    var body: some View {
        VStack {
            Image(systemName: "globe")
                .imageScale(.large)
                .foregroundStyle(.tint)
            Text("PF Services SDK Test App")
        }
        .padding()
    }
}

var deviceToken = ""

@_silgen_name("GetDeviceToken")  // vital for the function being visible from C
func GetDeviceToken() -> UnsafePointer<CChar>
{
    return (deviceToken as NSString).utf8String!
}

class AppController {
    
    private lazy var appleSignInCoordinator = AppleSignInCoordinator()

    @_silgen_name("SignInApple")  // vital for the function being visible from C
    func SignInApple() {
        appleSignInCoordinator.handleAuthorizationAppleID()
    }
    
    func runTestApp() {
        var testResult:Int32 = -1;

        let group = DispatchGroup()
        group.enter()

        DispatchQueue.main.async {
            testResult = MainApple()
            group.leave()
        }

        group.notify(queue: .main) {
            print("Finished Thread execution.")
            exit(testResult)
        }
    }
    
    func findTestTitleDataFile() {
        print("Finding testTitleData.json file location")
        let filePath = Bundle.main.url(forResource: "testTitleData", withExtension: "json")
        let path = filePath?.path()
        setenv("TESTTITLEDATA", path, 1)
    }
    
}

#if os(iOS)
class AppDelegate: UIResponder, UIApplicationDelegate {
    var appController: AppController = AppController()
    var allowUserInteraction: Bool = true

    func application(_ application: UIApplication,
               didFinishLaunchingWithOptions launchOptions:
                     [UIApplication.LaunchOptionsKey: Any]?) -> Bool {
#if PLAYFAB_AVOID_UI
    allowUserInteraction = false
    print("PLAYFAB_AVOID_UI macro is defined. App will run in non-interactive mode.")
#endif

        if (allowUserInteraction) {
            let notificationCenter = UNUserNotificationCenter.current()
            notificationCenter.requestAuthorization(options: [.alert, .sound, .badge]) { granted, error in
                if let error = error {
                    print(error.localizedDescription)
                }
                
                if (granted) {
                    DispatchQueue.main.async {
                        UIApplication.shared.registerForRemoteNotifications()
                    }
                }
            }
        }
        else {
            continueApplicationFlow()
        }

        return true
    }

    func application(_ application: UIApplication,
                didRegisterForRemoteNotificationsWithDeviceToken
                    incomingDeviceToken: Data) {
        deviceToken = incomingDeviceToken.map {String(format:"%02.2hhx",$0)}.joined()
        continueApplicationFlow()
    }

    func application(_ application: UIApplication,
                didFailToRegisterForRemoteNotificationsWithError
                    error: Error) {
        print(error.localizedDescription)
    }
    
    func continueApplicationFlow() {
        let _ = self.appController.findTestTitleDataFile()

        if (allowUserInteraction) {
            let _ = self.appController.SignInApple()
        }
        else {
            let _ = self.appController.runTestApp()
        }
    }
}
#elseif os(macOS)
class AppDelegate: NSResponder, NSApplicationDelegate {
    var appController: AppController = AppController()
    var allowUserInteraction: Bool = true

    func applicationDidFinishLaunching(_ notification: Notification) {
#if PLAYFAB_AVOID_UI
    allowUserInteraction = false
    print("PLAYFAB_AVOID_UI macro is defined. App will run in non-interactive mode.")
#endif

//        if (allowUserInteraction) {
//            let notificationCenter = UNUserNotificationCenter.current()
//            notificationCenter.requestAuthorization(options: [.alert, .sound, .badge]) { granted, error in
//                if let error = error {
//                    print(error.localizedDescription)
//                }
//
//                if (granted) {
//                    DispatchQueue.main.async {
//                        UIApplication.shared.registerForRemoteNotifications()
//                    }
//                }
//            }
//        }
//        else {
            continueApplicationFlow()
//        }
    }
    
    func application(_ application: NSApplication,
                didRegisterForRemoteNotificationsWithDeviceToken
                    incomingDeviceToken: Data) {
        deviceToken = incomingDeviceToken.map {String(format:"%02.2hhx",$0)}.joined()
        continueApplicationFlow()
    }

    func application(_ application: NSApplication,
                didFailToRegisterForRemoteNotificationsWithError
                    error: Error) {
        print(error.localizedDescription)
    }
    
    func continueApplicationFlow() {
        let _ = self.appController.findTestTitleDataFile()

        if (allowUserInteraction) {
            let _ = self.appController.SignInApple()
        }
        else {
            let _ = self.appController.runTestApp()
        }
    }
}
#endif
