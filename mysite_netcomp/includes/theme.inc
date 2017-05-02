<?php
/**
 * Builds markup for distributor information from parsed json data object
 *
 * @param string $data
 *  Distributor ID
 * @param object|bool $apiDistributorResults
 *  object containing distributor information
 * @return array|string
 */
function mysite_netcomp_build_distributor_info($data, $apiDistributorResults = FALSE)
    
{
    $dataPieces = explode('/', $data);
    $distributorID = array_pop($dataPieces);
    if (!$apiDistributorResults) {
        $apiDistributorResults = _mysite_netcomp_apiRequest("Distributor", "GET", "", $distributorID);
    }

    if ($apiDistributorResults == FALSE) {
        $output = "There were no results for this distributor search.";
        return $output;
    }

    $distData = array(
        'distName' => $apiDistributorResults->Name,
        'distDesc' => $apiDistributorResults->Description,
        'offices' => $apiDistributorResults->Offices,
    );
    foreach ($distData['offices'] as $key => $office) {
        $contactOutput = array();
        $office->mergedAddress = $office->Address1;
        if ($office->Address2) {
            $office->mergedAddress .= '<br>' . $office->Address2;
            if ($office->Address3) {
                $office->mergedAddress .= '<br>' . $office->Address3;

            }
        }
        foreach ($office->Contacts as $key => $contact) {
            $contactOutput[] = '<div class="contact-' . ($key + 1) . '">' . $contact->Name . ', ' . $contact->Title . '(' . l($contact->Email, 'mailto:' . $contact->Email) . ')</div>';
        }
        $distributorSummary = '<h2 class=<distName">' . $distData['distName'] . '</h2><div class="dist-summary">' . $distData['distDesc'] . '</div>';

        $officeOutput[] = '<div class="distributor-office">
    <h3 class="office-title">' . $office->Name . '</h3>
    <div class="office-info">
        <div class="location">
            <div class="label address">Address:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Telephone:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Fax:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Website:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>

        </div>
        <div class="additional-info">
            <div class="label address">Customer Base:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Language(s) Spoken:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Payment Method(s):</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Payment Terms:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>
            <div class="label address">Minimum Order:</div>
            <div class="entry address">' . $office->mergedAddress . '</div>


        </div>

    </div>
    <div class="contact-info">
        ' . implode('', $contactOutput) . '

    </div>


</div>';
    }

    $officeOutput = $distributorSummary . implode('', $officeOutput);
    return $officeOutput;
}

/**
 * Helper function for parsing price info
 *
 * @param object $data
 * @return array|string
 */
function mysite_netcomp_build_pricing_info($data){
    $priceOutput = array();
    if ($data->Count){
        $prices = $data->Prices;
        foreach ($prices as $key=>$price){

            $priceOutput[] = '<div class="quantity">'.$price->MinQty.'</div><div class="price">'.$price->Price.'</div>';
        }
    }
    $pricingHeaders = '<h3>Pricing</h3>
                        <h4>Currency:'. $data->Currency .'</h4>
                        <div class="label quantity"></div><div class="label price"></div>';
    $priceOutput = '<div class="price-wrapper">' . $pricingHeaders.implode('', $priceOutput) . '</div>';
    return $priceOutput;

}

/**
 * Helper function to parse regional offices
 * @param string $data
 *
 * @return mixed|string
 */
function mysite_netcomp_build_country_list($data){

    $officeCount = strpos($data, '&bull;');
    if ($officeCount){
        $data = substr_replace($data, '', 0, $officeCount);
        $data = '<div class="country-multi-wrapper"><a href="#">'.$data.'</a></div>';
    }
    else{
        $data = '<div class="country-wrapper">'.$data.'</div>';

    }
    $data = decode_entities($data);

    return $data;
}


//TODO: Make it so that the product is listed on each section on the results page?
//TODO Add an icon for if there is just one country or if there are more?