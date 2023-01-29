# Proiect-POO-Curs

Instructiuni de lucru:

O sa va trebuiasca VisualStudio si Cmake. (le gasiti pe net).  
Clonati repo-ul.

In folderul clonat trebuie sa creati de mana un folder gol numit "build" in el.  📁    
In interiorul folderului "build" deschideti un terminal unde bagati comanda "cmake .."    
Cmake va cauta in folderul parinte fisierul Cmake.txt si va crea proiectul VisualStudio in folderul vostru build.  ⚙  

Procesul asta trebuie repetat de fiecare data cand clonati sau dati pull la adaugarile altora la repository.  
Cand dati pull trebuie sa stergeti tot continutul din build si sa folositi iar cmake ca sa creati .sln-ul pe baza celor mai noi .cpp-uri.  
Mereu inainte sa faceti o schimbare asigurati-va ca lucrati pe versiunea curenta (dati pull) altfel ne lovim de merge conflicts.  

Tot ce lucram e in fisierul .sln din build care va deschide proiectul de VisualStudio.  ✔️  
Folderul build e ignorat de GitHub deoarece la clonarea pe alt PC se schimba ierarhia de foldere si chestiile din el nu mai merg. (de aia dam rebuild mereu)  
Deci .sln-ul nu se pastreaza in repo, dar orice schimbare pe care o faceti in el va aparea in .cpp-urile din folderul src. (dati save)  

In pricipiu vom lucra doar in fisierele din src/lab_m1/lab1 si orice commit va fi legat de ele.  📄

Daca stricati ceva nu-i jale si putem doar sa dam revert la ultimul commit bun.  👍
