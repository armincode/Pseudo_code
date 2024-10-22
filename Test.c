while (process_not_done == true) {
    // Step 1: Check if blockhash is still valid 
    if (getBlockHeight() <= lastValidBlockHeight) {               // basically, this condition is a method for checking blockhash expiration in Solana
        // Step 2: Check Transaction Status
        status = getSignatureStatuses(TX_id); // Get status to determine if it's already on the chain

        if (status == "finalized") {
            /* TX is finalized, exit loop. (Task accomplished but shouldn't use finalized commitment when sending TXs, 
            because "finalized" returns an older blockhash, reducing the available time before the blockhash expires, which could cause more failures, especially if the network is busy. 
            To check for absolute security and final confirmation, you can check for finalized status after the transaction is processed.) */
            break;
        }   

            
          else if (status == "confirmed") {
            // Transaction is confirmed but not yet finalized, optional retry or wait (usually safe to consider task accomplished when the status returns "confirmed")
            /* when this gets executed, the transaction has most likely landed. You can consider the process done now, but you could also check for "finalized" in later stages if you need more assurance */
            wait(some_delay);
        }

        
    } else {
        // Transaction is neither confirmed nor finalized, retry sending the transaction until accepted:
        try {
            sendTransaction(transaction);
            wait(some_delay); // Specify delay between each retry
        } catch (error) {
            // Handle network errors or transaction rejections
            log(error);
        }
    }
    

    // Step 3: Blockhash is expired, fetch new blockhash and resign transaction
    {
        const { blockhash, lastValidBlockHeight } = getLatestBlockhash();
        transaction.recentBlockhash = blockhash;
        signTransaction(transaction); // Re-sign transaction and fetch the new blockhash (Should use "getLatestBlockhash RPC Method" for fetching the new one)

    }
}
