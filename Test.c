WHILE (process_not_done = True) {
    // Step 1. check if blockhash is still Valid
    IF (getBlockHeight() <= lastValidBlockHeight) {    // this condition is simply how Solana checks for valid blockhashes
        // Step 2. Check Transaction Status
        status = getSignatureStatuses(transactionSignature);                //get status to determine if it's already on the network

        IF (status == "finalized") {
            // TX is finalized, exit loop (task accomplished)
            break;  
        } ELSE IF (status == "confirmed") {
            // Transaction is confirmed but not yet finalized, optional retry or wait (safe to consider task accomplished)
            wait(some delay);
        } ELSE {
            // Transaction is neither confirmed or finalized, Retry sending:
            try {
                submitTransaction(transaction);
                wait(some delay);  // Optional delay between retries
            } catch (error) {
                // handle network errors or transaction rejections
                log(error);
            }
        }
    } ELSE {
        // Step 3. blockhash is Expired, Fetch New Blockhash and Resign Transaction
        { blockhash, lastValidBlockHeight } = getRecentBlockhash();
        transaction.recentBlockhash = blockhash;
        signTransaction(transaction);  // Re-sign the transaction with the new blockhash
    }
}
