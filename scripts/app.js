angular.module('yun', ['ngResource'])
       .constant('apiNamespace', '/arduino')
       .service('temperatureService', function($resource, apiNamespace) {
          return $resource(apiNamespace + '/temperature');
       })
       .service('lightService', function($resource, apiNamespace) {
          return $resource(apiNamespace + '/light');
       })
       .controller('temperatureController', function($scope, $timeout, temperatureService) {
          var timeout;
          var poll = function() {
            temperatureService.get(function(response) {
                $scope.temperature = response;
                $scope.fillHeight = { height: response.temperature + '%' };
                timeout = $timeout(poll, 2500);
            });
          };
          poll();

          $scope.$on('destroy', function() {
            $timeout.cancel(timeout);
          });
       })
       .controller('lightController', function($scope, $timeout, lightService) {
          var timeout;
          var poll = function() {
            lightService.get(function(response) {
                $scope.light = response;
                $scope.opacity = { opacity: response.light / 1000 };
                $timeout(poll, 2500);
            });
          };
          poll();

          $scope.$on('destroy', function() {
            $timeout.cancel(timeout);
          });
       });
