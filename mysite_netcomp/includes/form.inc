<?php
/**
 * Generates custom Drupal form that is displayed in block
 *
 * @param array $form
 * @param array $form_state
 * @return mixed
 */
function mysite_netcomp_build_search_form($form, &$form_state){
    $form['search_pns'] = array(
        '#type' => 'fieldset',
        '#title' => t('Enter up to three product numbers')
    );
    $form['search_pns']['search_pn1'] = array(
        '#type' => 'textfield',
//        '#title' => t('Product ID')
    );
    $form['search_pns']['search_pn2'] = array(
        '#type' => 'textfield',
//        '#title' => t('Product ID')
    );
    $form['search_pns']['search_pn3'] = array(
        '#type' => 'textfield',
//        '#title' => t('Product ID')
    );
    $form['submit'] = array(
        '#type' => 'submit',
        '#value' => t('Search'),
        '#submit' => array('mysite_netcomp_search_submit')
    );
    $form['#validate'][] = 'mysite_netcomp_search_validate';
    if (isset($form_state['storage']['results'])){
        $form['results']= array(
            '#markup' => theme('nc_product_results', $form_state['storage']['results']),

        );
    }
        return $form;


}

/**
 * Validates and sanitizes form entry
 *
 * @param $form
 * @param $form_state
 */
function mysite_netcomp_search_validate($form, &$form_state){
    $submitValues = array('search_pn1', 'search_pn2', 'search_pn3');
    $noSearch = array();
    foreach ($submitValues as $value){
        $form_state['values'][$value] = preg_replace('/[\W\s]/','', $form_state['values'][$value]);
        $form_state['values'][$value] = filter_xss($form_state['values'][$value]);
        if ($form_state['values'][$value] == FALSE){
            $noSearch[$value] = TRUE;

        }

    }
    if (count($noSearch) >=3 ){
        drupal_set_message("Please enter at least one part number", 'warning');
    }


}

/**
 * Submit callback for search form. Executes API request based on values.
 *
 * @param $form
 * @param $form_state
 */
function mysite_netcomp_search_submit($form, &$form_state){
    $searchValues = array();
    foreach ($form_state['values'] as $value){
        if ($value){
            $searchValues[] = $value;
        }
    }
    $results = _mysite_netcomp_apiRequest("Search", "GET", "", $ids = $searchValues);

//    die(kpr($results));
    $form_state['storage']['results'] = array(
        '#value' => $results
    );
    $form_state['rebuild'] = TRUE;

}

