<?php

//TODO update for production
// Defining constants for API calls
define("DEFAULT_URL",       "https://apitest.netcomp.com");
define("DEFAULT_AUTH",      "Basic cG9tb25hZWxlY3Ryb25pY3M6MGFjMDI4NDIxZDEw");               // BASE64 encoded value, generate from your tools (REST Console) Basic Auth
define("TEST_AUTH",         "Basic MTAzNjQwMzpmMzJiMzZkZi00MDM0LTRjYjUtOTZlOC03NDdlMzZkZmQyNzI=");
define("DEFAULT_CONTENT",   "application/json");
define("DEFAULT_CLIENT_IP", "127.0.0.1");

/**
 * Passes credentials for API authorization. Gets login token
 *
 * @return mixed
 */
function _mysite_netcomp_get_authorization(){
    $headers = array(
        'Authorization' => DEFAULT_AUTH,
        'Content-Type' => DEFAULT_CONTENT,
    );
    $method = 'POST';
    $endpoint  = 'Login';
    $ids = '';

    $requestInfo = _mysite_netcomp_build_request($headers, $method, $endpoint, $ids);
    $httpRequest = drupal_http_request(DEFAULT_URL . $requestInfo['endpoint'] . $requestInfo['params'], $requestInfo['options']);
    $login_info = json_decode($httpRequest->data);
    return $login_info;

//    $_SESSION['OAuth'] = "Authorization: ".$login_info["AuthToken"];

}

/**
 * Builds API request
 *
 * @param array $headers
 *  Request headers from _mysite_netcomp_get_authorization
 * @param string $method
 *  Method type for request
 * @param string $endpoint
 *  DILP endpoint for API request
 * @param array|string $ids
 *  IDs for distributors
 * @return array
 */
function _mysite_netcomp_build_request($headers, $method, $endpoint, $ids){
    // This makes the $options array, which requires headers, method, data
    $options = array(
        "headers" => $headers,
        "method" => $method,
    );

    switch ($endpoint){
        case "Search":
            $endpoint = '/api/DILP/Search?';
            $numCount = count($ids);
            $data = array();


            for ($i = 1; $i <= $numCount; $i++){
                $key = 'PN'.$i;
                $data[$key] = check_plain(array_shift($ids));
            }
            $data['ClientIP'] = DEFAULT_CLIENT_IP;
            $params = http_build_query($data);


            break;

        case "Distributor":
            $endpoint = '/api/DILP/Distributor/';
            $data = array();
            if (is_array($ids)){
                $id = array_shift($ids);
                drupal_set_message(t("This distributor search contains more than one item, this search will be done only with the first distributor."), 'warning');
            }
            else {
                $id = $ids;
            }
            $data['ClientIP'] = DEFAULT_CLIENT_IP;
            $params = $id.'?'.http_build_query($data);

            break;

        case "Login":
            $endpoint = '/api/DILP/Login?TTL=3600';
            $params = '';
            break;
    }
    return array(
        'endpoint'=>$endpoint,
        'params' => $params,
        'options' => $options
        );
}

/**
 * Executes actual API request
 *
 * @param string $endpoint
 *  DILP endpoint for API request
 * @param string $method
 *  Method type for request
 * @param array $headers
 *  Request headers
 * @param string|array $ids
 *  IDs for distributors
 * @return bool|mixed
 */
function _mysite_netcomp_apiRequest($endpoint, $method, $headers = array(), $ids){

    if (count($headers) < 1 || $headers == FALSE){
        $headers = array(
            "Content-Type" => DEFAULT_CONTENT,
            "Authorization" => TEST_AUTH,
        );
    }
    if (isset($_COOKIE['NC_auth'])){
        $headers['Authorization']=$_COOKIE['NC_auth'];
    }
    else {
        $value = _mysite_netcomp_get_authorization();
        $value = $value->AuthToken;
        setcookie('NC_auth', TEST_AUTH, time()+3600, '/', $_SERVER['HTTP_HOST']);

    }
    $requestInfo = _mysite_netcomp_build_request($headers, $method, $endpoint, $ids);

    // builds the actual URL for the request
    $params = $requestInfo['params'];
    $endpoint = $requestInfo['endpoint'];
    $options = $requestInfo['options'];
    $httpRequest = drupal_http_request(DEFAULT_URL . $endpoint . $params, $options);

    // CHECKING FOR IF THERE ARE ERRORS WITH THE REQUEST

    // if it isn't fine to start with...
    if ($httpRequest->code !== '200'){
        // check if it is a result of not being authorized,
        // reset the cookie and retry the login request
        if ($httpRequest->code == '401'){
            $value = _mysite_netcomp_get_authorization();
            $value = $value->AuthToken;
            setcookie('NC_auth', $value, time()+3600, '/', $_SERVER['HTTP_HOST']);
            $options['headers']['Authorization'] = $value;
            $httpRetry = drupal_http_request(DEFAULT_URL . $endpoint . $params, $options);

            // if the result is ok, display the data
            if ($httpRetry->code == '200'){
                $results = json_decode($httpRetry->data);
            }

            // otherwise let the user know there was an issue
            else{
                drupal_set_message(t("There was a problem with this request. Please try again. If you continue to have issues, please send us a message"), 'error');
                $results = FALSE;
            }
        }
        // let the user know there was an issue
        else{
            drupal_set_message(t("There was a problem with this request. Please try again. If you continue to have issues, please send us a message"), 'error');
            $results = FALSE;
        }

    }
    else{

        $results = json_decode($httpRequest -> data);

    }
    return $results;




}