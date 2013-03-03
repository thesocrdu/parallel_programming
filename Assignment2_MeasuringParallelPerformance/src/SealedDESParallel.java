import java.util.Random;

import javax.crypto.Cipher;
import javax.crypto.SealedObject;
import javax.crypto.spec.SecretKeySpec;

/**
 * hw2
 *
 * Kyle Mercer
 * 520.320
 * 03/28/2013
 * HW#2
 * Phone#: 443-386-6211
 * JHED: kmercer5
 * Email: kmercer5@jhu.edu
 */

/**
 * Part 2
 * @author Kyle Mercer
 * 
 * This class parallelized the processes in SealedDES.
 */
public class SealedDESParallel implements Runnable {

    //Global start time
    private static long runstart;
    
    // Start key value
    private long keyStart;

    // number of keys to search
    private long numKeys;
    
    //SealedObject to decipher
    private static SealedObject targetObj;

    // Cipher for the class
    private Cipher des_cipher;

    // Key for the class
    private SecretKeySpec the_key = null;

    // Byte arrays that hold key block
    private byte[] deskeyIN = new byte[8];
    private byte[] deskeyOUT = new byte[8];

    /**
     * Constructor.
     * 
     * @param keySt
     *            the starting key value this obj (thread) will start at.
     * @param numKs
     *            the number of keys to search through.
     */
    public SealedDESParallel(long keySt, long numKs/*, SealedObject so*/) {
        this.keyStart = keySt;
        this.numKeys = numKs;
//        this.targetObj = so;
        try {
            des_cipher = Cipher.getInstance("DES");
        } catch (Exception e) {
            System.out.println("Failed to create cipher.  Exception: "
                    + e.toString() + " Message: " + e.getMessage());
        }
    }

    // Decrypt the SealedObject
    //
    // arguments: SealedObject that holds on encrypted String
    // returns: plaintext String or null if a decryption error
    // This function will often return null when using an incorrect key.
    //
    public String decrypt(SealedObject cipherObj) {
        try {
            return (String) cipherObj.getObject(the_key);
        } catch (Exception e) {
            //System.out.println("Failed to decrypt message. " + ". Exception: "
            //        + e.toString() + ". Message: " + e.getMessage());
        }
        return null;
    }

    // Encrypt the message
    //
    // arguments: a String to be encrypted
    // returns: a SealedObject containing the encrypted string
    //
    public SealedObject encrypt(String plainstr) {
        try {
            des_cipher.init(Cipher.ENCRYPT_MODE, the_key);
            return new SealedObject(plainstr, des_cipher);
        } catch (Exception e) {
            System.out.println("Failed to encrypt message. " + plainstr
                    + ". Exception: " + e.toString() + ". Message: "
                    + e.getMessage());
        }
        return null;
    }

    // Build a DES formatted key
    //
    // Convert an array of 7 bytes into an array of 8 bytes.
    //
    private static void makeDESKey(byte[] in, byte[] out) {
        out[0] = (byte) ((in[0] >> 1) & 0xff);
        out[1] = (byte) ((((in[0] & 0x01) << 6) | (((in[1] & 0xff) >> 2) & 0xff)) & 0xff);
        out[2] = (byte) ((((in[1] & 0x03) << 5) | (((in[2] & 0xff) >> 3) & 0xff)) & 0xff);
        out[3] = (byte) ((((in[2] & 0x07) << 4) | (((in[3] & 0xff) >> 4) & 0xff)) & 0xff);
        out[4] = (byte) ((((in[3] & 0x0F) << 3) | (((in[4] & 0xff) >> 5) & 0xff)) & 0xff);
        out[5] = (byte) ((((in[4] & 0x1F) << 2) | (((in[5] & 0xff) >> 6) & 0xff)) & 0xff);
        out[6] = (byte) ((((in[5] & 0x3F) << 1) | (((in[6] & 0xff) >> 7) & 0xff)) & 0xff);
        out[7] = (byte) (in[6] & 0x7F);

        for (int i = 0; i < 8; i++) {
            out[i] = (byte) (out[i] << 1);
        }
    }

    // Set the key (convert from a long integer)
    public void setKey(long theKey) {
        try {
            // convert the integer to the 8 bytes required of keys
            deskeyIN[0] = (byte) (theKey & 0xFF);
            deskeyIN[1] = (byte) ((theKey >> 8) & 0xFF);
            deskeyIN[2] = (byte) ((theKey >> 16) & 0xFF);
            deskeyIN[3] = (byte) ((theKey >> 24) & 0xFF);
            deskeyIN[4] = (byte) ((theKey >> 32) & 0xFF);
            deskeyIN[5] = (byte) ((theKey >> 40) & 0xFF);
            deskeyIN[6] = (byte) ((theKey >> 48) & 0xFF);

            // theKey should never be larger than 56-bits, so this should always
            // be 0
            deskeyIN[7] = (byte) ((theKey >> 56) & 0xFF);

            // turn the 56-bits into a proper 64-bit DES key
            makeDESKey(deskeyIN, deskeyOUT);

            // Create the specific key for DES
            the_key = new SecretKeySpec(deskeyOUT, "DES");
        } catch (Exception e) {
            System.out.println("Failed to assign key" + theKey
                    + ". Exception: " + e.toString() + ". Message: "
                    + e.getMessage());
        }
    }

    /**
     * @see java.lang.Runnable#run()
     */
    @Override
    public void run() {
        // Search for the right key
        long endKey = this.keyStart + this.numKeys;
        for (long i = this.keyStart; i < endKey; i++) {
            // Set the key and decipher the object
            this.setKey(i);
            String decryptstr = this.decrypt(SealedDESParallel.targetObj);

            // Does the object contain the known plaintext
            if ((decryptstr != null) && (decryptstr.indexOf("Hopkins") != -1)) {
                // Remote printlns if running for time.
                System.out.printf("Found decrypt key %016x producing message: %s\n", i, decryptstr);
                       
//                 System.out.println ( "Found decrypt key " + i +
//                 " producing message: " + decryptstr );
            }

            // Update progress every once in awhile.
            // Remote printlns if running for time.
            if (i % 100000 == 0) {
                long elapsed = System.currentTimeMillis() - SealedDESParallel.runstart;
                System.out.println("Searched key number " + i + " at "
                        + elapsed + " milliseconds.");
            }
        }

    }

    /**
     * Program entry point.
     * 
     * @param args
     *            Command line args.
     */
    public static void main(String[] args) {
        // BEGIN STARTUP stuff
        // Modified from original SealedDES class by Randal Burns
        if (2 != args.length) {
            System.out.println("Usage: java #threads key_size_in_bits");
            System.exit(-1);
        }

        // Get the arguments
        int numThreads = Integer.parseInt(args[0]);
        long keybits = Long.parseLong(args[1]);

        long maxkey = ~(0L);
        maxkey = maxkey >>> (64 - keybits);

        // Create a simple cipher
        SealedDESParallel enccipher = new SealedDESParallel(0,0);

        // Get a number between 0 and 2^64 - 1
        Random generator = new Random();
        long key = generator.nextLong();

        // Mask off the high bits so we get a short key
        key = key & maxkey;

        // Set up a key
        enccipher.setKey(key);

        // Generate a sample string
        String plainstr = "Johns Hopkins afraid of the big bad wolf?";

        // Encrypt
        // As far as our decrypter is concerned, sldObj is all we know
        SealedDESParallel.targetObj = enccipher.encrypt(plainstr);
//        SealedDESParallel.targetObj = sldObj;
        // END STARTUP stuff

        // BEGIN PARALLEL stuff
        long keysPerThread = maxkey / (long) numThreads;
        Thread[] threads = new Thread[numThreads];
        
        SealedDESParallel.runstart = System.currentTimeMillis();
        // First loop creates threads with flips / numThreads + 1 iterations
        // just in case numThreads does not perfectly divide into flips.
        for (long i = 0; i < maxkey % (long) numThreads; i++) {
            threads[(int) i] = new Thread(new SealedDESParallel(i
                    * keysPerThread, keysPerThread + 1));
            threads[(int) i].start();
        }
        // Make the rest of the threads with flips / numThreads coin flips
        for (long i = maxkey % (long) numThreads; i < (long) numThreads; i++) {
            threads[(int) i] = new Thread(new SealedDESParallel(i
                    * keysPerThread, keysPerThread));
            threads[(int) i].start();
        }
        
      //Join all threads back together
        for (int i = 0; i < numThreads; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                System.out.println("Thread interrupted.  Exception: " + e.toString() +
                        " Message: " + e.getMessage()) ;
                System.exit(-1);
            }
        }
        // END PARALLEL stuff
        
        long elapsed = System.currentTimeMillis() - SealedDESParallel.runstart;
        long keys = maxkey + 1;
        System.out.println ( "Completed search of " + keys + " keys at " + elapsed + " milliseconds.");
    }
}
