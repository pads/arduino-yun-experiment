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
            $scope.temperature = temperatureService.get(function() {
                timeout = $timeout(poll, 5000);
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
            $scope.light = lightService.get(function() {
                $timeout(poll, 5000);
            });
          };
          poll();

          $scope.$on('destroy', function() {
            $timeout.cancel(timeout);
          });          
       });       