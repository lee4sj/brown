package kj.audio;

public abstract interface KJAudioDataProvider
{
  public abstract int channels();

  public abstract void dispose();

  public abstract long endPosition();

  public abstract int frameCount();

  public abstract int frameSize();

  public abstract int frequency();

  public abstract boolean isDisposed();

  public abstract long position();

  public abstract long positionInMilliseconds();

  public abstract int sampleSize();

  public abstract void seek(long paramLong);

  public abstract long startPosition();

  public abstract boolean isEncrypted();

  public abstract boolean hasMoreFrames();

  public abstract void writeFrame(KJAudioDataConsumer paramKJAudioDataConsumer);
}

/* Location:           /home/sl136/course/cs166/secureplayer/SecurePlayer.jar
 * Qualified Name:     kj.audio.KJAudioDataProvider
 * JD-Core Version:    0.6.0
 */