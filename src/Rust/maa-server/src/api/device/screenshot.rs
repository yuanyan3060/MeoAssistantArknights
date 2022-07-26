use super::super::{Error, MaaManager};
use actix_web::{web, HttpResponse, Responder};
use serde::Deserialize;
use std::sync::Mutex;

#[allow(dead_code)]
#[derive(Deserialize)]
pub struct Req {
    id: i64,
}
pub async fn screenshot(
    req: web::Json<Req>,
    maa_manager: web::Data<Mutex<MaaManager>>,
) -> Result<impl Responder, Error> {
    let body = {
        let manager = maa_manager.lock().map_err(|_| Error::Internal)?;
        let maa = manager.get(req.id).ok_or(Error::InstanceNotFound)?;
        maa.screenshot()?
    };
    Ok(HttpResponse::Ok().body(body))
}
