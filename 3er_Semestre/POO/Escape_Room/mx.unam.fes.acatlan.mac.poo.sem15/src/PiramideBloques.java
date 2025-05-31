public class PiramideBloques {
    public static void main(String[] args) {
        long bloques = 19869345;
        int capas = 0;
        for (int i = 1; bloques >= i; i++) {
            bloques -= i;
            capas++;
        }
        System.out.println("La pirámide tendrá " + capas + " capas.");
    }
}


