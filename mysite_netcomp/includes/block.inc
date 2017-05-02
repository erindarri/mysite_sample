<?php


/**
 * Implements hook_block_info().
 */
function mysite_netcomp_block_info()
{
    $blocks = array();
    $blocks['mysite_nc_product_results'] = array(
        'info' => t('netcomp results on an individual product display')
    );
    $blocks['mysite_nc_search_form'] = array(
        'info' => t('netcomp form for search')
    );
    $blocks['mysite_nc_search_results'] = array(
        'info' => t('netcomp results returned from the search function')
    );
    return $blocks;

}

/**
 * Implements hook_block_view().
 */
function mysite_netcomp_block_view($delta = '')
{
    $block = array();
    switch($delta){
        case 'mysite_nc_product_results':
            $block['content'] = mysite_netcomp_individual_product_results_view();
            break;
        case 'mysite_nc_search_form':
            $block['content'] = mysite_netcomp_search_form_view();
            break;
        case 'mysite_nc_search_results':
            $block['content'] = mysite_netcomp_search_results_view();
            break;
    }
    return $block;
}

/**
 * Makes API call and generates markup for block on product pages
 * @param object|bool $apiResults
 *  API results from _mysite_netcomp_apiRequest
 * @return array
 *  standard block array
 */
function mysite_netcomp_individual_product_results_view($apiResults = FALSE){
    $currentNode = menu_get_object('node');
    $content = '';
    $block = array(
        "product_results" => array(
            '#type' => 'markup',
            '#markup' => $content
        )
    );

    if ((!$currentNode) || ($currentNode->type != 'product_display')){
        return $block;
    }
    else {
        $wrapper = entity_metadata_wrapper('node', $currentNode);
    }

    if (!$apiResults){

        $apiResults = _mysite_netcomp_apiRequest("Search", "GET", "", $ids = array($wrapper->field_model_number->value()));
    }

    if ($apiResults == FALSE) {
        $block['product_results']['#markup'] = "There were no results for this product search.";
    }
    else {
        $block['product_results']['#markup'] = theme('nc_product_results', array('#value' => $apiResults));

    }


    return $block;

}

/**
 * Builds search form for block
 *
 * @return array
 */
function mysite_netcomp_search_form_view(){
    $block = array();
    $searchForm = drupal_get_form('mysite_netcomp_build_search_form');
    $content = drupal_render($searchForm);
    $block = array(
        "product_results" => array(
            '#type' => 'markup',
            '#markup' => $content,
        )
    );

    return $block;

}

/**
 * Builds general search results for block
 * @return array
 */
function mysite_netcomp_search_results_view(){
    $block = array();
    $content = mysite_netcomp_build_search_results();
    $block = array(
        "product_results" => array(
            '#type' => 'markup',
            '#markup' => $content,
        )
    );
    return $block;

}