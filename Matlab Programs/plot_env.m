function plot_env(time, amp, env)
figure; plot(time,amp,'b-',time,imag(hilbert(amp)),'k--',time,env,'r:')
end