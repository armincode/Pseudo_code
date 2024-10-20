WHILE (process_not_done = True) {
    // Step 1. check if blockhash is still Valid
    
    IF (getBlockHeight() <= lastValidBlockHeight) {    // this condition is simply how Solana checks for valid blockhashes
        // Step 2. check Transaction Status
        status = getSignatureStatuses(TX id);                //get status to determine if it's already on the chain

        
        IF (status == "finalized") 
             /* TX is finalized, exit loop. (task accomplished but shouldn't use finalized commitment when sending TXs, 
            because "finalized" returns an older blockhash, reducing the available time before the blockhash expires, which could cause more failures, especially if network is busy. 
            But to check for absolute security and final confirmation, you can check for finalized status after the transaction is processed.) */
            break;  
        
           
        ELSE IF (status == "confirmed") 
            // Transaction is confirmed but not yet finalized, optional retry or wait (usually safe to consider task accomplished)
            wait(some delay); 
  
    }

         ELSE {
            // here Transaction is neither confirmed or finalized, so Retry Sending the Transaction until Accepted:
            try {
                sendTransaction(transaction);
                wait(some delay);  // It's better to specify delay between retries
             catch (error) {
                // handle network errors or transaction rejections
                log(error);
                }
            }
         }
     ELSE {
        // Step 3. blockhash is expired, Fetch new blockhash and Resign Transaction
        { blockhash, lastValidBlockHeight } = getRecentBlockhash();
        transaction.recentBlockhash = blockhash;
        signTransaction(transaction);  // Re-sign the transaction with the new blockhash

}
