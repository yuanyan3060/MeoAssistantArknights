use super::super::{Error, MaaManager};
use actix_web::{web, HttpResponse, Responder};
use serde_json::json;
use std::sync::Mutex;
pub async fn all(maa_manager: web::Data<Mutex<MaaManager>>) -> Result<impl Responder, Error> {
    let instances = {
        maa_manager
            .lock()
            .map_err(|_| Error::Internal)?
            .get_all_id()
    };
    Ok(HttpResponse::Ok().json(json!({
        "instances": instances,
        "result":  0,
    })))
}
