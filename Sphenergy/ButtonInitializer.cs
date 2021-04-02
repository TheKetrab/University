using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class ButtonInitializer : MonoBehaviour
{
    private void Awake()
    {
        foreach (Transform child in transform)
        {
            var component = child.GetComponent<Button>();
            if (component != null) InstallButton(child.gameObject);
        }
    }


    private void InstallButton(GameObject gObj)
    {
        AddAudioSources(gObj);
        
        var trigger = gObj.AddComponent<EventTrigger>();
        
        EventTrigger.Entry entry1 = new EventTrigger.Entry();
        entry1.eventID = EventTriggerType.PointerEnter;
        entry1.callback.AddListener( eventData => { PlayEnterSound(gObj); } );        
        trigger.triggers.Add(entry1);
        
        EventTrigger.Entry entry2 = new EventTrigger.Entry();
        entry2.eventID = EventTriggerType.PointerClick;
        entry2.callback.AddListener( eventData => { PlayClickSound(gObj); } );        
        trigger.triggers.Add(entry2);

    }

    private void AddAudioSources(GameObject gObj)
    {
        var audioSource_onEnter = gObj.AddComponent<AudioSource>();
        var audioSource_onClick = gObj.AddComponent<AudioSource>();

        
        AudioClip clip_onEnter = Resources.Load<AudioClip>("ButtonOnEnter");
        AudioClip clip_onClick = Resources.Load<AudioClip>("ButtonOnClick");
        
        audioSource_onEnter.clip = clip_onEnter;
        audioSource_onClick.clip = clip_onClick;
    }    

    
    void PlayEnterSound(GameObject gObj)
    {
        var audios = gObj.GetComponents(typeof(AudioSource));
        AudioSource sound = (AudioSource) audios[0];
        sound.Play();
    }
    
    private void PlayClickSound(GameObject gObj)
    {
        var audios = gObj.GetComponents(typeof(AudioSource));
        AudioSource sound = (AudioSource) audios[1];
        sound.Play();
    }
    
    
    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }
}
