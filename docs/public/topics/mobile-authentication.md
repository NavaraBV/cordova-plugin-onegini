# Mobile authentication

<!-- toc -->

## Introduction

The **Onegini Mobile Security Platform** offers a mobile authentication mechanism in a user friendly and secure way. You can take advantage of mobile authentication to add second factor authentication to your product, that can be used to improve the security of selected actions like logging into your website or accepting a transaction payment.

## Setup and requirements

The setup and requirements to enable mobile authentication differ per platform. For Android devices, the medium used to send push messages is **Google Cloud Messaging (GCM)**. For iOS devices, the **Apple Push Notification Service (APNS)** is used.
You will need a valid GCM Sender ID and/or APNS setup to receive push notifications in the Onegini Cordova Plugin.

## Push Configuration

To tell the Onegini Cordova Plugin about your GCM Sender ID, add the Sender ID to your `config.xml` as a preference.

```xml
<preference name="OneginiGcmSenderId" value="123456789012"/>
```

For iOS to receive push notifications. You will need to open `platforms/ios/MyApp.xcodeproj` in Xcode and configure APNS.
Make sure to enable `Push Notifications` under your App's Capabilities.

## Enrollment

Once push notifications have been set up, the Onegini Cordova plugin requires an authenticated or logged in user to enroll for mobile authentication. The user can be enrolled on only one application instance at a time. If the user has multiple mobile devices on which the application is installed, the user can only enroll for mobile authentication on one of these devices.

**Example code to enroll an authenticated user for mobile authentication:**

```js
onegini.mobileAuthentication.enroll()
    .then(() => {
      alert("Enrollment success!");
    })
    .catch((err) => {
      alert("Enrollment error!\n\n" + err.description);
    });
```

If you want to use mobile fingerprint authentication, you will need to register the fingerprint authenticator for the relevant user (see [User authentication with fingerprint](user-authentication-with-fingerprint.md)).

## Handling mobile authentication requests

You can configure different mobile authentication types in the Token Server [mobile authentication configuration](https://docs.onegini.com/token-server/topics/mobile-apps/mobile-authentication/mobile-authentication.html#configure-authentication-properties) panel. There are currently three different methods of authentication. These are **push**, **push with PIN**, and **push with fingerprint**. Only once the authentication type has been configured on the Token Server can it be used.

### Push

The first type of mobile authentication request is the most simple one, push (also referred to as "confirmation"). The user has the option to simply accept or deny a request. Typically, this is done by displaying two buttons to the user, although the choice of UI is free and left entirely to the app developer.

To handle a push mobile authentication request, the handler returned by [`onegini.mobileAuthentication.on`](../reference/mobileAuthentication/on.md) must be implemented for the parameter `"confirmation"`.

**Example code to handle push mobile authentication requests:**

```js
onegini.mobileAuthentication.on("confirmation")
    .onConfirmationRequest((actions, request) => {
      console.log("New mobile authentication request", request);

      // Ask user if they want to accept or deny the request. In this
      // example, the user accepts the request.
      let userAcceptedRequest = true;

      if (userAcceptedRequest) {
        actions.accept();
      } else {
        actions.deny();
      }
    })
    .onSuccess(() => {
      alert("Mobile authentication request success!");
    })
    .onError((err) => {
      alert("Mobile authentication request failed!\n\n" + err.description);
    });
```

The method `onConfirmationRequest` will be called when the Onegini Cordova plugin receives a mobile authentication request via push. The `actions.accept` and `actions.deny` methods allow the user to choose whether to accept or deny the mobile authentication request.

### Push with PIN

It is also possible to require the user to enter their PIN in order to confirm their identity before accepting a mobile authentication request. The push with PIN type of mobile authentication request adds another layer of security to the two factor authentication in your product.

Handling a push with PIN is similar to the simpler push, except the parameter given to [`onegini.mobileAuthentication.on`](../reference/mobileAuthentication/on.md) is `"pin"`. It also requires the implementation of a different, but familiar handler method.

**Example code to handle push with PIN mobile authentication requests:**

```js
onegini.mobileAuthentication.on("pin")
    .onPinRequest((actions, request) => {
      console.log("New mobile authentication request", request);

      // Ask the user if they want to accept or deny the request, and enter
      // their PIN in case of accept. In this example, the user accepts the
      // request and enters a PIN of "12346".
      let userAcceptedRequest = true;
      let pin = "12346";

      if (userAcceptedRequest) {
        actions.accept(pin);
      } else {
        actions.deny();
      }
    })
    .onSuccess(() => {
      alert("Mobile authentication request success!");
    })
    .onError((err) => {
      alert("Mobile authentication request failed!\n\n" + err.description);
    });
```

The `request` argument contains some helpful properties such as the number of failed and remaining PIN attempts. See [`onegini.mobileAuthentication.on`](../reference/mobileAuthentication/on.md) for more details.

### Push with fingerprint

For devices that support it, it is also possible to allow mobile authentication requests for fingerprint. Like push with PIN, this mobile authentication method adds an extra layer of security, while often being more convenient for the user than PIN. The user is required to scan their fingerprint before the mobile authentication request is confirmed.

The use of this mobile authentication method requires the fingerprint authenticator to have been registered for the user. See [User authentication with fingerprint](user-authentication-with-fingerprint.md) topic guide for more information.

Handling a push with fingerprint is very similar to the other types of mobile authentication requests. See [`onegini.mobileAuthentication.on`](../reference/mobileAuthentication/on.md) for more details.

**Example code to handle push with fingerprint mobile authentication requests:**

```js
onegini.mobileAuthentication.on("fingerprint")
    .onPinRequest((actions, request) => {
      console.log("New mobile authentication request", request);

      // We assume that on fallback, the user still accepts the request
      // and provides a pin of "12346".
      let pin = "12346"

      actions.accept(pin);
    })
    .onFingerprintRequest((actions, request) => {
      console.log("New mobile authentication request", request);

      // Ask the user if they want to accept or deny the request. After
      // they accept, the OS will prompt them for a fingerprint scan. In
      // this example, the user accepts the request.
      let userAcceptedRequest = true;

      if (userAcceptedRequest) {
        actions.accept();
      } else {
        actions.deny();
      }
    })
    .onFingerprintCaptured(() => {
      // Only called on Android
      console.log("Fingerprint captured, waiting for verification.");
    })
    .onFingerprintFailed(() => {
      // Only called on Android
      console.log("Fingerprint failed! Please try again.");
    })
    .onSuccess(() => {
      alert("Mobile authentication request success!");
    })
    .onError((err) => {
      alert("Mobile authentication request failed!\n\n" + err.description);
    })
```

The details of these handler methods are exactly the same as explained in the fingerprint authentication topic guide linked above. In particular, `onFingerprintCaptured` and `onFingerprintFailed` are only available on Android devices, due to the more restrictive nature of Touch Id for iOS. Additionally, care must be taken to implement `onPinRequest`, as the Onegini Cordova plugin will perform a fallback to pin in the case of multiple failed fingerprint requests.