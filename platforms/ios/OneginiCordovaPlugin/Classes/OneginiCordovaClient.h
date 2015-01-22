//
//  OneginiCordovaClient.h
//  OneginiCordovaPlugin
//
//  Created by Eduard on 13-01-15.
//
//

#import <Cordova/CDV.h>
#import <OneginiSDKiOS/OneginiSDK.h>

@interface OneginiCordovaClient : CDVPlugin <OGAuthorizationDelegate, OGResourceHandlerDelegate>

@property (strong, nonatomic) OGOneginiClient *oneginiClient;
@property (strong, nonatomic) OGConfigModel *configModel;

@property (copy, nonatomic) NSString *authorizeCommandTxId;
@property (copy, nonatomic) NSString *confirmPinCommandTxId;
@property (copy, nonatomic) NSString *fetchResourceCommandTxId;

/** FOR TESTING PURPOSE ONLY */
- (void)clearTokens:(CDVInvokedUrlCommand *)command;

/** FOR TESTING PURPOSE ONLY */
- (void)clearCredentials:(CDVInvokedUrlCommand *)command;

/**
 Initialize this 'OGOneginiClient' with a valid config model and SSL pinning certificates.
 Command params:
 Dictionary cconfig
 Array certificates
 */
- (void)initWithConfig:(CDVInvokedUrlCommand *)command;

/**
 Initiate the authorization flow
 */
- (void)authorize:(CDVInvokedUrlCommand *)command;

/**
 Change the registered PIN. The callback is invoked with a request to show a PIN change entry dialog.
 */
- (void)changePin:(CDVInvokedUrlCommand *)command;

/**
 This is not a direct entry point and only valid to be called when a delegate askForPinWithVerification is requested.
 When the askForPinWithVerification is invoked then the user PIN entry is forwarded back to the OneginiClient by this method.
 
 Command params:
 String pin
 String verifyPin
 Boolean retry
 */
- (void)confirmPinWithVerification:(CDVInvokedUrlCommand *)command;

/**
 This is not a direct entry point and only valid to be called when a delegate askForPin is requested.
 When the askForPin is invoked then the user PIN entry is forwarded back to the OneginiClient by this method.
 
 Command arguments:
 String pin
 Boolean retry
 */
- (void)confirmPin:(CDVInvokedUrlCommand *)command;

/**
 This is not a direct entry point and only valid to be called when a delegate askForPinChangeWithVerification is requested.
 When the askForPinChangeWithVerification is invoked then the user PIN entries are forwarded back to the OneginiClient by this method.
 
 Command params:
 String currentPin
 String newPin
 String verifyNewPin
 Boolean retry
 */
- (void)confirmChangePinWithVerification:(CDVInvokedUrlCommand *)command;

/**
 Cancel the PIN change. This is not a rollback of a changed PIN.
 It should be called when the user PIN change input dialog is cancelled by the user.
 */
- (void)cancelPinChange:(CDVInvokedUrlCommand *)command;

/**
 Logout will invalidate the current session.
 */
- (void)logout:(CDVInvokedUrlCommand *)command;

/**
 Disconnect the device, this will clear the refresh token and access token.
 Client credentials remain untouched.
 */
- (void)disconnect:(CDVInvokedUrlCommand *)command;

/**
 Fetches a specific resource. 
 The access token validation flow is invoked if no valid access token is available.
 
 Command arguments:
 String path
 Array scopes
 String requestMethod, GET, PUT, POST or DELETE
 String parameterEncoding, FORM, JSON or PROPERTY
 Dictionary request parameters
 */
- (void)fetchResource:(CDVInvokedUrlCommand *)command;

/**
 Fetches a specific resource anonymously using a client access token. 
 The access token validation flow is invoked if no valid access token is available.
 
 Command arguments:
 String path
 Array scopes
 String requestMethod, GET, PUT, POST or DELETE
 String parameterEncoding, FORM, JSON or PROPERTY
 Dictionary request parameters
 */
- (void)fetchAnonymousResource:(CDVInvokedUrlCommand *)command;
@end