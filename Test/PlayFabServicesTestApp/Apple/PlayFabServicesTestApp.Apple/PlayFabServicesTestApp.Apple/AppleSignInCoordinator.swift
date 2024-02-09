//
//  AppleSignInCoordinator.swift
//  PlayFabServicesTestApp.iOS
//

import AuthenticationServices

var identityToken = ""

@_silgen_name("GetIdentityToken")  // vital for the function being visible from C
func GetIdentityToken() -> UnsafePointer<CChar>
{
    return (identityToken as NSString).utf8String!
}

class AppleSignInCoordinator: NSObject, ASAuthorizationControllerDelegate {
    private lazy var appController: AppController = AppController()

    // Shows Sign in with Apple UI
    func handleAuthorizationAppleID() {
        let appleIDProvider = ASAuthorizationAppleIDProvider()
        let request = appleIDProvider.createRequest()
        request.requestedScopes = [.fullName, .email]
        
        let authorizationController = ASAuthorizationController(authorizationRequests: [request])
        authorizationController.delegate = self
        authorizationController.performRequests()
    }
    
    // Delegate methods
    public func authorizationController(controller: ASAuthorizationController, didCompleteWithAuthorization authorization: ASAuthorization) {
        switch authorization.credential {
            case let appleIDCredential as ASAuthorizationAppleIDCredential:
                identityToken = String(data: appleIDCredential.identityToken!, encoding: .utf8)!

                if (identityToken != "") {
                    print("Successfully got Identity Token.")
                    appController.runTestApp()
                }
            default:
                break
        }
    }

    public func authorizationController(controller: ASAuthorizationController, didCompleteWithError error: Error) {
        print(error.localizedDescription)
    }
}
