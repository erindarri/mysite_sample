<?php

if  (isset($variables['#value']) && $variables['#value']  ){
    $results = $variables['#value'];
}
?>


<?php if (isset($results)): ?>
    <div class="inventory-search-wrapper">
        <?php foreach ($results->SearchedParts as $key => $result): ?>
            <div class="inventory-header">
                <div class="part-number">Part Number</div>
                <div class="updated">Updated</div>
                <div class="country">Country</div>
                <div class="quantity">Quantity</div>
                <div class="pricing">Pricing</div>
                <div class="shopping-cart">Buy</div>
                <div class="distributor">Distributor</div>
            </div>
            <?php foreach ($result->Parts as $key => $item): ?>
                <?php $partInventory = array(
                'partNum'=> $item->PartNum,
                'partDate' => $item->PartDate,
                'country' => $item->CountryCode,
                'quantity' => $item->Quantity,
                'pricing' => $item->PartPrice,
                'shoppingCart' => $item->CartLink->URL,
                'distributorName' => $item->Distributor,
                'distributorUrl' => $item->DistributorURI
                );
                ?>

                <div class="inventory-item">
                    <div class="summary">
                        <div class="part-number"><?php print $partInventory['partNum'] ?></div>
                        <div class="updated"><?php print $partInventory['partDate'] ?></div>
                        <div class="country"><?php print mysite_netcomp_build_country_list($partInventory['country']) ?></div>
                        <div class="quantity"><?php print $partInventory['quantity'] ?></div>
                        <div class="pricing"><?php print mysite_netcomp_build_pricing_info($partInventory['pricing']) ?></div>
                        <div class="shopping-cart"><?php print l(t('buy'), $partInventory['shoppingCart']) ?></div>
                        <div class="distributor"><?php //TODO remove all hardcoded urls!
                            print l(t($partInventory['distributorName']), 'http://google.com') ?></div>
                    </div>

                    <div class="distributor-offices">
                        <?php print mysite_netcomp_build_distributor_info($partInventory['distributorUrl']) ?>
                    </div>


                </div>


            <?php endforeach; ?>
        <?php endforeach; ?>


    </div>

<?php endif; ?>


