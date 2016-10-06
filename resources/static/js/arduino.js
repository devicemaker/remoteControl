angular.module('remote.servo', ['ui.bootstrap']);
angular.module('remote.servo').controller('ConnectController', function ($scope, $interval, $window) {
    $scope.ws_url = "ws://localhost:8090/events/"; // use external ip address instead of localhost
    $scope.connected = false;
    $scope.loggedin = "not logged in";
    $scope.position = "0";
    $scope.power = "turned off";
    $scope.login = "";
    $scope.code = "";
    $scope.inactivity = "<none>";
    $scope.timer_value = 0;

    $scope.websocket = {}

    function countDownTimer() {
        if ($scope.power == "turned on") {
            $scope.timer_value--;
        }
    }

    function startTimer() {
        $interval(countDownTimer, 1000, 0, true);
    }

    function resetTimer() {
        $scope.inactivity = "<none>";
        $scope.timer_value = 60;
    }

    function getQueryParameter ( parameterName, defaultValue ) {
      var queryString = window.top.location.search.substring(1);
      var parameterName = parameterName + "=";
      if ( queryString.length > 0 ) {
        begin = queryString.indexOf ( parameterName );
        if ( begin != -1 ) {
          begin += parameterName.length;
          end = queryString.indexOf ( "&" , begin );
            if ( end == -1 ) {
            end = queryString.length
          }
          return unescape ( queryString.substring ( begin, end ) );
        }
      }
      return defaultValue;
    }

    function showSentMessage(message) {
    }

    function showReceivedMessage(message) {
    }

    $scope.sendLoginMessage = function(password) {
      $scope.sendCommandMessage('login:' + password)
    }

    $scope.sendOnMessage = function() {
      $scope.sendCommandMessage('O')
    }

    $scope.sendOffMessage = function() {
      $scope.sendCommandMessage('F')
    }

    $scope.sendIncreaseMessage = function() {
      $scope.sendCommandMessage('I')
    }

    $scope.sendDecreaseMessage = function() {
      $scope.sendCommandMessage('D')
    }

    $scope.sendKeepGoingMessage = function() {
      $scope.sendCommandMessage('K')
    }

    $scope.sendCommandMessage = function(message) {
        resetTimer();
        showSentMessage(message);
        $scope.websocket.send(message);
    }


    $scope.connect = function() {
        $scope.websocket = new WebSocket($scope.ws_url);

        $scope.websocket.onopen = function(evt) { onOpen(evt) };
        $scope.websocket.onclose = function(evt) { onClose(evt) };
        $scope.websocket.onmessage = function(evt) { onMessage(evt) };
        $scope.websocket.onerror = function(evt) { onError(evt) };

        function onOpen(evt) {
            $scope.connected = true;
            startTimer();
            $scope.$apply()
        }

        function onClose(evt) { $scope.connected = false;
          $scope.$apply() }

        function onMessage(event) {
            showReceivedMessage(event.data);
            if (event.data.indexOf("pos:") == 0) {
                $scope.position = event.data.substring(4)
                $scope.$apply()
            }
            else if (event.data.indexOf("on:") == 0) {
                onState = event.data.substring(3)
                if (onState == "1") {
                    resetTimer();
                    $scope.power = "turned on";
                }
                else {
                    $scope.power = "turned off";
                    $scope.timer_value = 0;
                }
                $scope.$apply()
            } else if (event.data.indexOf("loggedin:true") == 0) {
                $scope.loggedin = "currently logged in";
                $scope.$apply()
            } else if (event.data.indexOf("timeout") == 0) {
                $scope.inactivity = new Date().toString();
                $scope.$apply()
            }

        }

        function onError(evt) {
            showReceivedMessage("error:" + evt.data);
            $scope.$apply();
        }
    }

    $scope.disconnect = function() {
        $scope.websocket.close();;
    }

    function pageLoaded() {
        $scope.connect();
    }

    window.onload = pageLoaded;

  });